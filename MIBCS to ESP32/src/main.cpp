/**
 * @file main.cpp
 * @author JmmyGun
 * @brief MIBCS>BLE>ESP32 SERIAL MONITOR
 * @version 0.1
 * @date 2023-10-28
 *
 * @copyright Copyright (c) 2023
 *
 * @section Descrizione
 * BRIDGE da MIBCS all'ESP32 tramite BLE i dati ricevuti
 *
 * @section Errori
 * ERRORI NON VINCOLANTI:
 * - 02:29:17:361 -> [144336][E][BLERemoteCharacteristic.cpp:289] retrieveDescriptors(): esp_ble_gattc_get_all_descr: Sconosciuto
 * - 02:29:17:371 -> [144337][E][BLERemoteCharacteristic.cpp:289] retrieveDescriptors(): esp_ble_gattc_get_all_descr: Sconosciuto
 *
 * @section Tested On
 * Questo programma è stato testato con successo (non proprio) sulla bilancia Xiaomi Mi Scale 2.
 *
 * @section Features Future
 * Le seguenti funzionalità sono previste per il futuro (forse) sviluppo del programma:
 * - Implementation of MQTT for data communication.
 * - Calcolo del grasso viscerale.
 * - Calcolo dell'Indice di Massa Corporea (BMI).
 * - Calcolo della Massa Magra (utilizzando l'impedenza).
 * - Percentuale di grasso corporeo.
 * - Percentuale di acqua corporea.
 * - Massa ossea.
 * - Massa muscolare.
 * - Grasso viscerale.
 * - Calcolo del Metabolismo Basale (BMR).
 * - Calcolo del peso ideale.
 * - Calcolo della massa grassa da perdere/aumentare.
 * - Percentuale di proteine nel corpo.
 * - Tipo di corporatura.
 * - Implementazione database JSON per la gestione dei dati dell'utente che utilizza la bilancia.
 */

#include <Arduino.h>
#include "BLEDevice.h"

// Variabili globali
bool trovato = false;

// UUID di base
// Servizio della bilancia
static BLEUUID srvUUID("0000181b-0000-1000-8000-00805f9b34fb");
// Caratteristica di misurazione del peso
static BLEUUID chrUUID("00002a9c-0000-1000-8000-00805f9b34fb");
static BLEAdvertisedDevice *bilancia;
static BLERemoteCharacteristic *caratteristicaRemota;
static boolean connetti = false;
static boolean connesso = false;
static int tempo = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Inizia scansione BLE per dispositivo MIBCS");
  BLEDevice::init("");
}

/**
 * Funzione di callback per notifica/indicazione della caratteristica
 */
static void callbackCaratteristica(BLERemoteCharacteristic *caratteristicaRemota, uint8_t *dati, size_t lunghezza, bool isNotifica)
{
  /**
   * Analisi dati ricevuti e calcolo del peso
   * (che cagata le capre fanno meglio...)
   * PARTE DA MIGLIORARE
   */
  boolean temporaneo = true;
  int tempoRicevuto = dati[3];

  if (tempo == 0)
  {
    tempo = tempoRicevuto;
  }
  else
  {
    if (tempoRicevuto > tempo)
    {
      temporaneo = false;
    }
  }
  double peso = 0;
  peso = (dati[11] + dati[12] * 256) / 200.0;

  // Calcolo dell'impedenza
  int impedenza = dati[9] + dati[10] * 256;

  // Controllo se il peso è stabile
  bool isStabilized = dati[1] & (1 << 2);

  if (isStabilized && impedenza < 65500 && impedenza != 0)
  {
    Serial.print("Peso: ");
    Serial.print(peso);
    Serial.print(" Kg - ");
    Serial.print("Impedenza: ");
    Serial.print(impedenza);
    Serial.println(" Ohm (Definitivo)");
  }
  else
  {
    Serial.print("Peso: ");
    Serial.print(peso);
    if (impedenza < 65500 && impedenza != 0)
    {
      Serial.print(" Kg - Impedenza: ");
      Serial.print(impedenza);
      Serial.println(" Ohm (Provvisorio)");
    }
    else
    {
      Serial.println(" Kg - Impedenza non rilevata (Provvisorio)");
    }
  }
}

/**
 * Classe di callback per ciascun dispositivo trovato durante la scansione
 */
class callbackDispositivo : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice dispositivoTrovato)
  {
    if (!trovato && dispositivoTrovato.haveServiceUUID() && dispositivoTrovato.isAdvertisingService(srvUUID))
    {
      if (dispositivoTrovato.getName() != "MIBCS")
      {
        Serial.print(".");
      }
      else
      {
        Serial.println("  Trovato!");
        BLEDevice::getScan()->stop();
        Serial.println("Interrompi la scansione e connettiti alla bilancia");
        bilancia = new BLEAdvertisedDevice(dispositivoTrovato);
        connetti = true;
        trovato = true;
      }
    }
    else
    {
      Serial.print(".");
    }
  }
};

/**
 * Classe di callback per gli eventi conn/disconn del dispositivo
 */
class callbackClient : public BLEClientCallbacks
{
  void onConnect(BLEClient *clientBLE)
  {
  }

  /**
   * Va gestita meglio secondo me lo scan
   * deve partire lo scan con un pulsante o comando
   * per evitare connessioni e disconnessioni continue
   * *Vedi commento nel "void loop()"
   */
  void onDisconnect(BLEClient *clientBLE)
  {
    Serial.println("Disconnesso. Ritorno alla scansione...");
    clientBLE->disconnect(); // Aggiungi questa riga
    connesso = false;
    trovato = false;
    connetti = false;

    // Ritarda di 10 secondi con conteggio alla rovescia
    for (int i = 5; i > 0; i--)
    {
      Serial.print("Ritorno alla scansione in ");
      Serial.print(i);
      Serial.println(" secondi...");
      delay(1000);
    }
  }
};

bool connessioneConBilancia()
{
  Serial.println("Avvio della connessione con la bilancia:");
  BLEClient *clientBLE = BLEDevice::createClient();
  Serial.println("    Client BLE creato");
  clientBLE->setClientCallbacks(new callbackClient());
  // Connetti al server BLE remoto.
  clientBLE->connect(bilancia);
  Serial.println("    Connesso alla bilancia");
  // Ottieni un riferimento al servizio che serve nel server BLE remoto.
  BLERemoteService *servizioRemoto = clientBLE->getService(srvUUID);
  if (servizioRemoto == nullptr)
  {
    Serial.println("    Errore: impossibile trovare il servizio");
    clientBLE->disconnect();
    return false;
  }
  Serial.println("    Servizio trovato");
  caratteristicaRemota = servizioRemoto->getCharacteristic(chrUUID);
  if (caratteristicaRemota == nullptr)
  {
    Serial.print("    Impossibile trovare la caratteristica");
    clientBLE->disconnect();
    return false;
  }
  Serial.println("    Caratteristica trovata");
  Serial.println("    Impostazione del callback per notifica/indicazione");
  caratteristicaRemota->registerForNotify(callbackCaratteristica);
  return true;
}

void loop()
{

  /**
   * Attenzione: La scansione BLE continua può esaurire la batteria della bilancia
   * a causa delle continue connessioni/discooneesioni ad essa e causare
   * lievi discrepanze nei conteggi HCI.
   * Avvia la scansione solo quando necessario con un comando o un pulsante
   * per preservare la batteria della bilancia.
   * Connessioni/disconnessioni continue portano al blocco del codice.
   *
   * Esempio di errore durante connessioni successive alla bilancia:
   * 02:27:18:077 -> Inizio connessione con la bilancia:
   * 02:27:18:081 ->     Client BLE creato
   *   ***PROGRAMMA BLOCCATO***
   */

  if (!trovato)
  {
    BLEScan *scansioneBLE = BLEDevice::getScan();
    scansioneBLE->setAdvertisedDeviceCallbacks(new callbackDispositivo());
    scansioneBLE->setInterval(1349);
    scansioneBLE->setWindow(449);

    scansioneBLE->setActiveScan(true);
    scansioneBLE->start(1, false);
  }

  if (connetti && !connesso)
  {
    delay(1000); // Ritarda prima di tentare la connessione
    connesso = connessioneConBilancia();
  }
}