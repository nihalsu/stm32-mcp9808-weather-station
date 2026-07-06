# STM32 High-Resolution ADC Voltage Measurement

This project demonstrates analog-to-digital conversion (ADC) on an STM32 microcontroller. It continuously samples an analog input, converts the 16-bit raw data into a voltage value, and utilizes the Board Support Package (BSP) for board-specific hardware management.

## Project Overview
![ADC Ölçüm Şeması](adc_diagram.jpeg)

## Features
- **High-Resolution Sampling:** Configured for 16-bit resolution to ensure precise voltage measurement.
- **Continuous Conversion:** Uses continuous mode to keep the ADC running in the background.
- **Calibration:** Performs automatic calibration at startup for measurement accuracy.
- **BSP Integration:** Utilizes STM32 BSP libraries for LED and user button management.

## Technical Details
- **Microcontroller:** STM32
- **Peripheral:** ADC1 (Analog-to-Digital Converter)
- **Resolution:** 16-bit (0-65535)
- **Reference Voltage:** 3.3V
- **Communication:** BSP COM port interface

## How It Works
1. **Calibration:** `HAL_ADCEx_Calibration_Start` is called to compensate for internal offset errors.
2. **ADC Loop:** The ADC continuously polls for conversion results.
3. **Calculation:** The raw 16-bit integer is mapped to a float voltage value using the formula:
   $Voltage = \frac{ADC_{value} \times 3.3}{65535.0}$

## License
Copyright (c) 2026 STMicroelectronics. Provided as-is.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

# STM32 Yüksek Çözünürlüklü ADC Voltaj Ölçümü

Bu proje, bir STM32 mikrokontrolcüsü üzerinde Analog-Dijital Dönüştürücü (ADC) kullanımını göstermektedir. Analog girişi sürekli örnekleyerek 16-bit ham veriyi gerçek voltaj değerine dönüştürür ve donanım yönetimi için BSP (Board Support Package) kütüphanelerini kullanır.

## Özellikler
- **Yüksek Çözünürlüklü Örnekleme:** Hassas voltaj ölçümü için 16-bit çözünürlükte yapılandırılmıştır.
- **Sürekli Dönüşüm:** ADC'nin arka planda sürekli çalışması için "Continuous Mode" kullanılmıştır.
- **Kalibrasyon:** Ölçüm doğruluğunu artırmak için başlangıçta otomatik kalibrasyon gerçekleştirilir.
- **BSP Entegrasyonu:** LED ve buton yönetimi için standart STM32 BSP kütüphaneleri kullanılmıştır.

## Teknik Detaylar
- **Mikrokontrolcü:** STM32
- **Çevre Birimi:** ADC1 (Analog-Dijital Dönüştürücü)
- **Çözünürlük:** 16-bit (0-65535)
- **Referans Voltajı:** 3.3V
- **Haberleşme:** BSP COM port arayüzü

## Çalışma Mantığı
1. **Kalibrasyon:** İçsel sapma hatalarını gidermek için `HAL_ADCEx_Calibration_Start` fonksiyonu kullanılır.
2. **ADC Döngüsü:** ADC, sonuçları sürekli olarak yoklar (poll).
3. **Hesaplama:** Ham 16-bit tamsayı değer, aşağıdaki formül ile float voltaj değerine dönüştürülür:
   $Voltaj = \frac{ADC_{degeri} \times 3.3}{65535.0}$

## Lisans
Telif Hakkı (c) 2026 STMicroelectronics. "Olduğu gibi" sağlanmıştır.
