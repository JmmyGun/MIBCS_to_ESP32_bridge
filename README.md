# MIBCS to ESP32 bridge

**MIBCS to ESP32 Bridge** è un bridge tra la bilancia Mi Body Composition Scale 2 (MIBCS) e un ESP32. Il bridge consente di acquisire e monitorare i dati della bilancia MIBCS tramite BLE e visualizzarli su un ESP32 attraverso il monitor seriale.

## Caratteristiche

- Connessione BLE alla bilancia MIBCS.
- Monitoraggio dei dati del peso e dell'impedenza.
- Visualizzazione dei dati attraverso il monitor seriale.
- Testato con successo (quasi) sulla bilancia Xiaomi Mi Scale 2.

## Clone

Per clonare il repository, utilizza il seguente comando:

`git clone https://github.com/JmmyGun/MIBCS_to_ESP32_bridge.git`

## Descrizione

Bridge tra la bilancia Mi Body Composition Scale 2 (MIBCS) e l'ESP32 attraverso BLE. Consente di acquisire e monitorare i dati del peso in modo efficiente. Durante il funzionamento, potrebbero verificarsi errori non vincolanti *(sono documentati nel codice)* bisogna aggiustarlo ma funziona

## Test

Il programma è stato testato (quasi con successo) sulla bilancia Xiaomi Mi Scale 2.

## Funzionalità future

Le seguenti funzionalità sono previste per il futuro sviluppo del programma:

- Implementazione di MQTT per la comunicazione dei dati.
- Calcolo del grasso viscerale.
- Calcolo dell'Indice di Massa Corporea (BMI).
- Calcolo della Massa Magra (utilizzando l'impedenza).
- Percentuale di grasso corporeo.
- Percentuale di acqua corporea.
- Massa ossea.
- Massa muscolare.
- Grasso viscerale.
- Calcolo del Metabolismo Basale (BMR).
- Calcolo del peso ideale.
- Calcolo della massa grassa da perdere/aumentare.
- Percentuale di proteine nel corpo.
- Tipo di corporatura.
- Implementazione database JSON per la gestione dei dati dell'utente che utilizza la bilancia.

## Ringraziamenti

Il progetto MIBCS to ESP32 Bridge si basa sulla documentazione relativa agli UUID e ai byte, tratta dalla documentazione per Xiaomi Mi Scale 2 nel repository [wiecosystem/Bluetooth](https://github.com/wiecosystem/Bluetooth/).
Puoi trovare ulteriori dettagli sulla struttura degli UUID e dei byte consultando la documentazione [qui](https://github.com/wiecosystem/Bluetooth/blob/master/doc/devices/huami.health.scale2.md).

Un ringraziamento speciale va a [wiecosystem](https://github.com/wiecosystem) per la documentazione dettagliata e il lavoro svolto [wiecosystem/Bluetooth](https://github.com/wiecosystem/Bluetooth).

## Licenza

Questo progetto è distribuito sotto la licenza MIT. Per ulteriori dettagli, si prega di consultare il file LICENSE nel repository.

© 2023, [JmmyGun](https://github.com/JmmyGun).

