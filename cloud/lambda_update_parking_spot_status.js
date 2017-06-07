/**
 * This lambda function updates parking spot status in DynamoDB with the new value from the request.
 */

'use strict';

const doc = require('dynamodb-doc');
const dynamo = new doc.DynamoDB();

const PARKING_SPOTS_TABLE = 'unionCollege_iot_parkingSpots';

exports.handler = (event, context, callback) => {
    const done = (err, res) => callback(null, {
        statusCode: err ? '400' : '204',
        body: err ? err.message : undefined
    });

    dynamo.updateItem(
        {
            TableName: PARKING_SPOTS_TABLE,
            Key: {
                "id": event.id
            },
            UpdateExpression: 'set occupied = :occupied',
            ExpressionAttributeValues: {
                ':occupied': event.newStatus
            }
        }, 
        done
    );
};
