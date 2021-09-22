#include "AHT10.h"

AHT10 AHT10::AHT10(uint8_t endereco) {
   _endereco = end;
}

bool AHT10::begin(void) {
  Wire.begin();
  Wire.setClock(100000);   
}


/***********************************************************************/
uint8_t AHT10::lerDadosRaw(){
    
    Wire.beginTransmission(_endereco);
    
    #if (ARDUINO) >= 100
    Wire.write(AHT10_INICIA_MEDICAO);                                
    Wire.write(AHT10_MEDICAO_DADOS);                                 
    Wire.write(0x00);                                                
    #else
    Wire.send(AHT10_INICIA_MEDICAO);
    Wire.send(AHT10_MEDICAO_DADOS);
    Wire.send(0x00);
    #endif

    if (Wire.endTransmission(true) != 0) return AHT10_ERRO;                    //erro, houve colisão no barramento I2C

    if (getBitCalibracao() != 0x01)      return AHT10_ERRO;            
    if (getBusyBit(AHT10_USAR_DADO_LIDO) != 0x00) delay(delay_medicao); //measurement delay

    /* ler 6 bits do sensor */
    Wire.requestFrom(_endereco, 6, true);                                        

    if (Wire.available() != 6)
    {
        _bufferDadosRaw[0] = AHT10_ERRO;
        return AHT10_ERRO;
    }

    /* read 6 bytes from "wire.h" rxBuffer */
    #if (ARDUINO) >= 100
    for (uint8_t i = 0; i < 6 ; i++)
    {
        _bufferDadosRaw[i] = Wire.read();
    }
    #else
    for (uint8_t i = 0; i < 6 ; i++)
    {
        _bufferDadosRaw[i] = Wire.receive();
    }
    #endif

    return true;
}
/***********************************************************************/



/***********************************************************************/
float AHT10::lerUmidade(bool lerI2C)
{
    if (lerI2C == AHT10_FORCA_LER_DADO)
    {
        if (lerDadosRaw() == AHT10_ERRO) return AHT10_ERRO;   //force to read data to _bufferDadosRaw & error handler
    }

    if (_bufferDadosRaw[0] == AHT10_ERRO) return AHT10_ERRO; //error handler, collision on I2C bus

    uint32_t dadoRaw = (((uint32_t)_bufferDadosRaw[1] << 16) | ((uint16_t)_bufferDadosRaw[2] << 8) | (_bufferDadosRaw[3])) >> 4; //20-bit raw humidity data

    float umidade = (float)dadoRaw * 0.000095;

    if (umidade < 0)   return 0;
    if (umidade > 100) return 100;
    return umidade;
}
/***********************************************************************/

/***********************************************************************/
float AHT10::lerTemperatura(bool lerI2C)
{
  if (lerI2C == AHT10_FORCA_LER_DADO)
  {
    if (lerDadosRaw() == AHT10_ERRO) return AHT10_ERRO;   //force to read data to _bufferDadosRaw & error handler
  }

  if (_bufferDadosRaw[0] == AHT10_ERRO) return AHT10_ERRO; //error handler, collision on I2C bus

  uint32_t temperaura = ((uint32_t)(_bufferDadosRaw[3] & 0x0F) << 16) | ((uint16_t)_bufferDadosRaw[4] << 8) | _bufferDadosRaw[5]; //20-bit raw temperature data

  return (float)temperatura * 0.000191 - 50;
}
/***********************************************************************/


/***********************************************************************/
uint8_t AHT10::getBusyBit(bool lerI2C)
{
    if (lerI2C == AHT10_FORCA_LER_DADO) _bufferDadosRaw[0] = readStatusByte(); //force to read status byte

    if (_bufferDadosRaw[0] != AHT10_ERROR) 
        return bitRead(_bufferDadosRaw[0], 7); //get 7-th bit
    
    return AHT10_ERRO;
}
/***********************************************************************/


/***********************************************************************/
uint8_t AHT10::getBitCalibracao(bool lerI2C)
{
    if (lerI2C == AHT10_FORCA_LER_DADO) _bufferDadosRaw[0] = readStatusByte(); //force to read status byte

    if (_bufferDadosRaw[0] != AHT10_ERROR) 
        return bitRead(_bufferDadosRaw[0], 3); //get 3-rd bit
    
    return AHT10_ERRO;
}
/***********************************************************************/


// ------------------- Funçãoes de Conversão ---------------------- //
float AHT10::converteCelsiusParaFahrenheit(float TEMPE_CELSIUS) {
  return (TEMPE_CELSIUS * 1.8 + 32);
}

float AHT10::converteCelsiusParaKelvin(float TEMPE_CELSIUS) {
  return (TEMPE_CELSIUS + 273);
}

float AHT10::converteKelvinParaFahrenheit(float TEMPE_KELVIN) {
  return ((TEMPE_KELVIN - 273) * 1.8 + 32);
}

float AHT10::converteFahrenheitParaKelvin(float TEMPE_FAHRENHEIT) {
  return ((TEMPE_FAHRENHEIT - 32) * (5/9) + 273);
}