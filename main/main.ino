#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// BLE Server name
#define BLE_SERVER_NAME "ESP32 BLE Bridge"

// BLE Service and Characteristic UUIDs
#define BLE_SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define BLE_RX_CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define BLE_TX_CHARACTERISTIC_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pRxCharacteristic;
BLECharacteristic *pTxCharacteristic;

bool deviceConnected = false;

// Callback function for connection status changes
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        // Restart advertising to allow reconnection
        pServer->getAdvertising()->start();
    }
};

// Callback function for RX characteristic write events
class MyRxCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        String rxValue = pCharacteristic->getValue();
        if (rxValue.length() > 0) {
            // Convert String to std::string and forward the received data to the serial port (RX pin)
            std::string rxValueStd = rxValue.c_str();
            for (int i = 0; i < rxValueStd.length(); i++) {
                Serial.write(rxValueStd[i]);
            }
        }
    }
};

void setup() {
    Serial.begin(115200);

    // Initialize BLE
    BLEDevice::init(BLE_SERVER_NAME);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    pService = pServer->createService(BLE_SERVICE_UUID);

    // Create the RX Characteristic
    pRxCharacteristic = pService->createCharacteristic(
                                         BLE_RX_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyRxCallbacks());

    // Create the TX Characteristic
    pTxCharacteristic = pService->createCharacteristic(
                                         BLE_TX_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());

    // Increase the MTU size
    BLEDevice::setMTU(512);

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // Recommended value for compatibility with older devices
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

void loop() {
    // Check for data on the serial port (RX pin)
    if (Serial.available()) {
        uint8_t data = Serial.read();
        if (deviceConnected) {
            pTxCharacteristic->setValue(&data, 1);
            pTxCharacteristic->notify();
        }
    }

    //delay(10); // Delay to avoid busy waiting
}
