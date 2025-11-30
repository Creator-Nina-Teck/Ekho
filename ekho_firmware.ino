#include <Arduino.h>
#include <LittleFS.h>
#include <driver/i2s.h>


#define BTN_REC  2
#define BTN_PLAY 3
#define BTN_DEL  4

#define I2S_WS   7
#define I2S_SD   6
#define I2S_SCK  5


#define SAMPLE_RATE   16000
#define BUFFER_SIZE   1024
#define MAX_RECORDING_TIME 5000  // 5 seconds

int fileIndex = 0;


void setupI2S() {
    i2s_config_t config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 512
    };

    i2s_pin_config_t pins = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = -1,
        .data_in_num = I2S_SD
    };

    i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pins);
}


void recordAudio() {
    String filename = "/rec" + String(fileIndex) + ".raw";
    File f = LittleFS.open(filename, "w");

    if (!f) return;

    Serial.println("Recording...");

    uint8_t buffer[BUFFER_SIZE];
    uint32_t start = millis();

    while (millis() - start < MAX_RECORDING_TIME) {
        size_t bytesRead;
        i2s_read(I2S_NUM_0, buffer, BUFFER_SIZE, &bytesRead, portMAX_DELAY);
        f.write(buffer, bytesRead);
    }

    f.close();
    fileIndex++;

    Serial.println("Done.");
}


void playRandom() {
    if (fileIndex == 0) {
        Serial.println("No recordings yet.");
        return;
    }

    int n = random(0, fileIndex);
    String filename = "/rec" + String(n) + ".raw";

    File f = LittleFS.open(filename, "r");
    if (!f) return;

    Serial.println("Playing: " + filename);

    uint8_t buffer[BUFFER_SIZE];

    while (f.available()) {
        size_t len = f.read(buffer, BUFFER_SIZE);
        size_t written;
        i2s_write(I2S_NUM_0, buffer, len, &written, portMAX_DELAY);
    }

    f.close();
}

void deleteAll() {
    LittleFS.format();
    fileIndex = 0;
    Serial.println("All recordings deleted.");
}

void setup() {
    Serial.begin(115200);

    pinMode(BTN_REC, INPUT_PULLUP);
    pinMode(BTN_PLAY, INPUT_PULLUP);
    pinMode(BTN_DEL, INPUT_PULLUP);

    LittleFS.begin(true);
    setupI2S();

    Serial.println("EKHO READY");
}

void loop() {

    if (!digitalRead(BTN_REC)) {
        delay(200);
        recordAudio();
    }

    if (!digitalRead(BTN_PLAY)) {
        delay(200);
        playRandom();
    }

    if (!digitalRead(BTN_DEL)) {
        delay(200);
        deleteAll();
    }
}
