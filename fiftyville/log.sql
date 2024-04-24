-- Keep a log of any SQL queries you execute as you solve the mystery.

.tables

.schema crime_scene_reports

SELECT description FROM crime_scene_reports
WHERE year = 2023 AND day = 28 AND month = 7
AND street LIKE '%Humphrey Street%';

-- 10:15am at the Humphrey Street bakery
-- Littering took place at 16:36. No known witnesses

.schema interviews

SELECT name,transcript FROM interviews
WHERE year = 2023 AND day = 28 AND month = 7;

-- | Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- | Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- | Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.


-- | Ruth    | Sometime within ten minutes of the theft (10:15 - 10:25), I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
SELECT activity,license_plate,hour,minute FROM bakery_security_logs
WHERE year = 2023 AND day = 28 AND month = 7
AND hour = 10
AND minute > 14 AND minute < 26;
-- suspects

-- +----------+---------------+------+--------+
-- | activity | license_plate | hour | minute |
-- +----------+---------------+------+--------+
-- | exit     | 5P2BI95       | 10   | 16     |
-- | exit     | 94KL13X       | 10   | 18     |
-- | exit     | 6P58WS2       | 10   | 18     |
-- | exit     | 4328GD8       | 10   | 19     |
-- | exit     | G412CB7       | 10   | 20     |
-- | exit     | L93JTIZ       | 10   | 21     |
-- | exit     | 322W7JE       | 10   | 23     |
-- | exit     | 0NTHK55       | 10   | 23     |
-- +----------+---------------+------+--------+


-- | Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
. schema atm_transactions

SELECT account_number,transaction_type,amount FROM atm_transactions
WHERE year = 2023 AND day = 28 AND month = 7
AND atm_location LIKE '%Leggett Street%'
ORDER BY(amount) DESC;
-- suspects

--  account_number   transaction_type   amount
-- | 16153065       | withdraw         | 80     |
-- | 76054385       | withdraw         | 60     |
-- | 49610011       | withdraw         | 50     |
-- | 28500762       | withdraw         | 48     |
-- | 26013199       | withdraw         | 35     |
-- | 81061156       | withdraw         | 30     |
-- | 28296815       | withdraw         | 20     |
-- | 25506511       | withdraw         | 20     |

-- | Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
. schema phone_calls

SELECT caller,receiver,duration FROM phone_calls
WHERE year = 2023 AND day = 28 AND month = 7
AND duration < 60;
-- suspects

-- |     caller     |    receiver    | duration |
-- | (130) 555-0289 | (996) 555-8899 | 51       |
-- | (499) 555-9472 | (892) 555-8872 | 36       |
-- | (367) 555-5533 | (375) 555-8161 | 45       |
-- | (499) 555-9472 | (717) 555-1342 | 50       |
-- | (286) 555-6063 | (676) 555-6554 | 43       |
-- | (770) 555-1861 | (725) 555-3243 | 49       |
-- | (031) 555-6622 | (910) 555-3251 | 38       |
-- | (826) 555-1652 | (066) 555-9701 | 55       |
-- | (338) 555-6650 | (704) 555-2131 | 54       |

-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow (july-29). The thief then asked the person on the other end of the phone to purchase the flight ticket.

.schema flights
.schema airports
.schema passengers

SELECT flights.id,hour,minute,flights.origin_airport_id,
origin_join.full_name,origin_join.abbreviation,origin_join.city,
flights.destination_airport_id,
destination_join.full_name,destination_join.abbreviation,destination_join.city FROM flights
JOIN airports AS origin_join ON origin_join.id = flights.origin_airport_id
JOIN airports AS destination_join ON destination_join.id = flights.destination_airport_id
WHERE year = 2023 AND day = 29 AND month = 7
ORDER BY(hour),(minute);

-- destination city New York City
-- | id | hour | minute | origin_airport_id |          full_name          | abbreviation |    city    | destination_airport_id |              full_name              | abbreviation |     city      |
-- +----+------+--------+-------------------+-----------------------------+--------------+------------+------------------------+-------------------------------------+--------------+---------------+
-- | 36 | 8    | 20     | 8                 | Fiftyville Regional Airport | CSF          | Fiftyville | 4                      | LaGuardia Airport                   | LGA          | New York City |

.schema passengers

SELECT passport_number,seat FROM passengers
WHERE flight_id = 36;
-- suspects

-- +-----------------+------+
-- | passport_number | seat |
-- +-----------------+------+
-- | 7214083635      | 2A   |
-- | 1695452385      | 3B   |
-- | 5773159633      | 4A   |
-- | 1540955065      | 5C   |
-- | 8294398571      | 6C   |
-- | 1988161715      | 6D   |
-- | 9878712108      | 7A   |
-- | 8496433585      | 7B   |
-- +-----------------+------+

.schema people

SELECT * FROM people
WHERE people.passport_number IN (
SELECT passengers.passport_number FROM passengers
WHERE flight_id = 36
);
--suspects

-- +--------+--------+----------------+-----------------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate |
-- +--------+--------+----------------+-----------------+---------------+
-- | 395717 | Kenny  | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 398010 | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 467400 | Luca   | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 560886 | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 651714 | Edward | (328) 555-1152 | 1540955065      | 130LD9Z       |
-- | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 953679 | Doris  | (066) 555-9701 | 7214083635      | M51FA04       |
-- +--------+--------+----------------+-----------------+---------------+

SELECT * FROM people
WHERE people.passport_number IN (
SELECT passengers.passport_number FROM passengers
WHERE flight_id = 36)
AND people.license_plate IN (
SELECT bakery_security_logs.license_plate FROM bakery_security_logs
WHERE bakery_security_logs.year = 2023 AND bakery_security_logs.day = 28 AND bakery_security_logs.month = 7
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute > 14 AND bakery_security_logs.minute < 26
);

-- suspects
-- +--------+--------+----------------+-----------------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate |
-- +--------+--------+----------------+-----------------+---------------+
-- | 398010 | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 467400 | Luca   | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | 560886 | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+--------+----------------+-----------------+---------------+

SELECT account_number,person_id FROM bank_accounts
WHERE person_id IN (
398010,467400,560886,686048
);
-- suspects bank accounts

-- +----------------+-----------+
-- | account_number | person_id |
-- +----------------+-----------+
-- | 49610011       | 686048    |
-- | 28500762       | 467400    |
-- +----------------+-----------+

-- | Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

-- suspect's bank accounts that made withdraw on Leggett Street on crime morning
SELECT account_number,transaction_type FROM atm_transactions
WHERE year = 2023 AND day = 28 AND month = 7
AND atm_location LIKE '%Leggett Street%'
AND account_number IN(49610011,28500762);

-- +----------------+------------------+
-- | account_number | transaction_type |
-- +----------------+------------------+
-- | 28500762       | withdraw         |
-- | 49610011       | withdraw         |
-- +----------------+------------------+

-- | Raymond | As the thief was leaving the bakery, they CALLED someone who talked to them for less than a minute.
-- suspect's possible phone numbers
-- (367) 555-5533
-- (389) 555-5198


SELECT caller,receiver FROM phone_calls
WHERE year = 2023 AND day = 28 AND month = 7
AND duration < 60
AND (caller = '(367) 555-5533' OR caller = '(389) 555-5198');

-- thief phone number: (367) 555-5533
-- accomplice phone number: (375) 555-8161
-- +----------------+----------------+
-- |     caller     |    receiver    |
-- +----------------+----------------+
-- | (367) 555-5533 | (375) 555-8161 |
-- +----------------+----------------+

SELECT * FROM people
WHERE people.phone_number IN (
'(367) 555-5533','(375) 555-8161'
);

-- thief: Bruce
-- accomplice: Robin
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 864400 | Robin | (375) 555-8161 | NULL            | 4V16VO0       |
-- +--------+-------+----------------+-----------------+---------------+
