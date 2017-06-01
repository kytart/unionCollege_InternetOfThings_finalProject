#define PARKING_SPOT_EMPTY 0
#define PARKING_SPOT_OCCUPIED 1

#define PARKING_SPOTS_COUNT 3


typedef struct ParkingSpot {
  int id;
  bool occupied;
  int pin;
  ParkingSpot();
} ParkingSpot;


/**
 * Initialize array of parking spots.
 * The number of parking spots is defined by PARKING_SPOTS_COUNT.
 * Parking spots are assigned ids, starting from 1.
 * Also, a pin is assigned to each parking lot from an array of pins, provided as a function argument.
 * Those pins should be analog input pins and a photoresistor should be connected to each pin.
 */
ParkingSpot * initializeParkingSpots(const int pins[]);


/**
 * Read a real current status of a parking spot based on readings from assigned pin.
 */
bool readParkingSpotStatus(ParkingSpot parkingSpot);


/**
 * Connect's to AWS API and updates status of a parking spot.
 */
bool updateParkingSpotStatusOnServer(const ParkingSpot parkingSpot, const int newStatus);


/**
 * Connect's to AWS API and returns ID of an empty parking spot.
 * If no parking spot is available, return -1.
 */
const int getEmptyParkingSpotIdFromServer();

