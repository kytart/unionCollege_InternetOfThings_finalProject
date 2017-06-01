# UnionCollege: ECE-481-01 Internet of Things - Final Project

Smart parking lot, powered by Arduino and AWS.

This is a part of a miniature smart city, built by students in the class. Every student built a different part of the city with different functionality.


## Parking spots

Photoresistors are used to detect if the parking spot is empty or occupied.


## Incoming car detection

Entrance to the parking spot is barred by a gate, operated by a servo. There is a break-beam sensor in front of the gate to detect incoming car. An empty parking spot is then assigned to the car and the gate opens to let it through.


## Amazon Web Services

Services used in this project:

* API Gateway
* Lambda function
* DynamoDB

### State of parking spots

Everytime a parking spot state changes, a PUT request is sent to REST API end-point on AWS. That triggers a Lambda function which updates state of the parking spot in DynamoDB. Finally, state is updated internally on Arduino to maintain it localy.

### Asign parking spot to a new car

When an incoming car is detected by the break-beam sensor, a GET request is sent to REST API end-point on AWS. That triggers a Lambda function which fetches an empty parking spot, which is the closest to the entrance, from DynamoDB.


## Arduino

[Adafruit Feather M0 WiFi with ATWINC1500](https://www.adafruit.com/product/3010) was used for this project.
