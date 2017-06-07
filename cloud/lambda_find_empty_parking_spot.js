/**
 * This lambda function finds an empty parking spot in the database and returns it.
 */

'use strict';

const doc = require('dynamodb-doc');
const dynamo = new doc.DynamoDB();

const PARKING_SPOTS_TABLE = 'unionCollege_iot_parkingSpots';
const INDEX_NAME = 'occupied-priority-index';

exports.handler = (event, context, callback) => {
    const done = (err, res) => {
        let statusCode;
        let body;
        
        if(err) {
            statusCode = 400;
            body = err.message;
        } else if(res.Items.length > 0) {
            statusCode = 200;
            body = res.Items[0];
        } else {
            statusCode = 404;
        }
        
        callback(null, {
            statusCode: statusCode,
            body: body
        });
    }

    dynamo.query(
        {
            TableName: PARKING_SPOTS_TABLE,
            IndexName: INDEX_NAME,
            KeyConditionExpression: 'occupied = :occupied',
            ExpressionAttributeValues: {
                ':occupied': 0
            },
            ScanIndexForward: true,
            Limit: 1,
            ProjectionExpression: 'id'
        },
        done
    );
};
