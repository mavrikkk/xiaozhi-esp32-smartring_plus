#include "board.h"
#include "audio_codecs/es8311_audio_codec.h"
#include "displays/qspi_display.h"
#include "esp_log.h"

static const char *TAG = "SmartRingPlus";

class SmartRingPlusBoard : public Board {
private:
    I2cBus *i2c_bus_ = nullptr;

public:
    SmartRingPlusBoard() {
        // 1. Инициализация системной шины I2C (для звука и тачскрина)
        i2c_bus_ = new I2cBus(I2C_NUM_0, 8, 9); // SDA=8, SCL=9
    }

    virtual void Initialize() override {
        ESP_LOGI(TAG, "Initializing SmartRing-Plus Peripherals...");
        Board::Initialize();
    }

    virtual AudioCodec* GetAudioCodec() override {
        static Es8311AudioCodec audio_codec(
            i2c_bus_, 
            0x18, // I2C адрес ES8311
            48,   // MCLK
            21,   // BCLK
            17,   // WS / LRCLK
            16,   // DOUT
            18,   // DIN
            45,   // PA Enable Pin
            false
        );
        return &audio_codec;
    }

    virtual Display* GetDisplay() override {
        static QspiDisplay display(
            11, // CS
            10, // SCL / CLK
            12, // D0
            13, // D1
            15, // D2
            14, // D3
            39, // RST
            46, // BL (Подсветка)
            360, 360, // Разрешение
            true, // Круглый экран
            0 // Инверсия цвета / разворот
        );
        return &display;
    }
};

DECLARE_BOARD(SmartRingPlusBoard);
