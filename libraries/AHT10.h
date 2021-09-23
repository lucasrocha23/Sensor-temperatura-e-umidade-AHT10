#ifndef AHT10_h
#define AHT10_h

#include <Arduino.h>
#include <avr/pgmspace.h>     
#include <Wire.h>

#define AHT10_ENDERECO_0X38  0x38  
#define COMANDO_INIT_AHT10   0xE1  
#define AHT10_INICIA_MEDICAO 0xAC
#define AHT10_MEDICAO_DADOS  0x33
#define AHT10_SOFT_RESET_CMD 0xBA
#define AHT10_NORMAL_CMD     0xA8
#define AHT10_INICIALIZA_CAL 0x08
#define AHT10_FORCA_LER_DADO true
#define AHT10_USAR_DADO_LIDO false
#define AHT10_ERRO           0xFF

#define delay_medicao       80
#define delay_inicializacao 40
#define delay_comandos      350
#define delay_soft_reset    20

class AHT10
{
    public:
        AHT10(uint8_t address = AHT10_ENDERECO_0X38);
        bool begin();

        uint8_t  lerDadosRaw();
        float    lerTemperatura(bool readI2C = AHT10_FORCA_LER_DADO);
        float    lerUmidade(bool readI2C = AHT10_FORCA_LER_DADO);
        bool     softReset();
    
    // ---------- Funçãoes de Conversão ------------- //
        float   converteCelsiusParaFahrenheit(float TEMPE_CELSIUS);
        float   converteCelsiusParaKelvin(float TEMPE_CELSIUS);
        float   converteKelvinParaFahrenheit(float TEMPE_KELVINS);
        float   converteFahrenheitParaKelvin(float TEMPE_FAHRENHEIT);
        bool    setModoNormal();

    private:
        uint8_t _endereco;
        uint8_t _bufferDadosRaw[6] = {AHT10_ERRO, 0, 0, 0, 0, 0};

        uint8_t  getBitCalibracao(bool readI2C = AHT10_FORCA_LER_DADO);
        uint8_t  getBusyBit(bool readI2C = AHT10_FORCA_LER_DADO);
        uint8_t  readStatusByte();
        bool     habilitaFactoryCalCoeff();
};
#endif