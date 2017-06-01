#define PARKING_SPOT_EMPTY 0
#define PARKING_SPOT_OCCUPIED 1


/**
 * Connect's to AWS API and updates status of a parking spot.
 */
const bool updateParkingSpotStatus(const int id, const int newStatus);


/**
 * Connect's to AWS API and returns ID of an empty parking spot.
 * If no parking spot is available, return -1.
 */
const int getEmptyParkingSpotId();

