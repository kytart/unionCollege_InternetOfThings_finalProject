
/**
 * Return, if a car is detected at the entrance by the break-beam.
 * Argument pin should be a digital input pin, connected to the break-beam collector.
 */
bool readCarDetectionStatus(const int pin);


/**
 * Open the gate, operated by servo.
 */
void openGate(const int pin);


/**
 * Close the gate, operated by servo.
 */
void closeGate(const int pin);

