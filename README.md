# LoRa Half-Duplex

## Descrição
Comunicação LoRa simples usando ESP32.  
O dispositivo alterna entre enviar e receber mensagens. Para funcionar corretamente, é necessário ao menos 2 módulos idênticos, e pode-se usar o mesmo código em ambos.

- Envia `PING` a cada 5s (Debug)
- Recebe mensagens LoRa
- Permite envio pelo Serial

## Configuração

- **Frequência:** 433 MHz  
  - Dentro dos limites do módulo, ambos devem estar na mesma frquência

- **Spreading Factor:** 7  
  - Mais rápido mas menor alcance, suficiente para testes

- **Bandwidth:** 125 kHz  
  - Padrão estável (equilíbrio taxa/alcance, se aumentar, diminui alcance e aumenta velocidade de transmissão)

- **Coding Rate:** 4/5  
  - Menos redundância (mais suscetível a erros), maior velocidade

- **Potência:** 17 dBm  
  - Alto alcance para testes (máximo do módulo é 18 dBm)

## Observação
Usar **3.3V apenas** no módulo LoRa.
