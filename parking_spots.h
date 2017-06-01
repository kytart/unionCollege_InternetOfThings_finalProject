#define PARKING_SPOT_EMPTY 0
#define PARKING_SPOT_OCCUPIED 1

#define PARKING_SPOTS_COUNT 5


typedef struct ParkingSpot {
  int id;
  bool empty;
  ParkingSpot();
} ParkingSpot;


/**
 * Initialize array of parking spots.
 * The number of parking spots is defined by PARKING_SPOTS_COUNT.
 * Parking spots are assigned ids, starting from 1.
 */
ParkingSpot * initializeParkingSpots();


/**
 * Connect's to AWS API and updates status of a parking spot.
 */
const bool updateParkingSpotStatus(const int id, const int newStatus);


/**
 * Connect's to AWS API and returns ID of an empty parking spot.
 * If no parking spot is available, return -1.
 */
const int getEmptyParkingSpotId();

