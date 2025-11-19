// Joystick pins
int yaxis = A0;
int xaxis = A1;
int button = A2;

// Calibration values for neutral drift range
int maxXValue = 0;
int minXValue = 1023;
int maxYValue = 0;
int minYValue = 1023;
int numReading = 10; // no.of reading for initial calibration of neutral drift range

// Raw analog readings
int xReading;
int yReading;
// Mapped movement values
int xValue;
int yValue;

// my atempt at more "dynamic" mouse movement
// speed scalling
int xScale = 1;
int yScale = 1;
// Counters used to detect how long joystick is held to the extreme
int xCount = 0;
int yCount = 0;
// How many loop cycles before acceleration increases
int count = 20;

void setup()
{

    pinMode(yaxis, INPUT);
    pinMode(xaxis, INPUT);
    pinMode(button, INPUT);
    Serial.begin(9600);

    // calibration
    // after first powering the arduino the joystick should be in the neutral position untouched for about a second

    // cheap joystick module have some fluctuation in the analog reading even when there is no movement causing drift
    // to eleminate that we find the min and max value at neutral and only move when the reading exceeds that
    for (int i = 0; i < numReading; i++)
    {
        xReading = analogRead(xaxis);
        yReading = analogRead(yaxis);

        // find the min and max drift for the neutral positon of the joystick
        if (xReading > maxXValue)
        {
            maxXValue = xReading;
        }
        else if (xReading < minXValue)
        {
            minXValue = xReading;
        }

        if (yReading > maxYValue)
        {
            maxYValue = yReading;
        }
        else if (yReading < minYValue)
        {
            minYValue = yReading;
        }
        // small dely for stability
        delay(10);
    }
}

void loop()
{

    // simple debounce button
    static int lastButtonState = HIGH;
    static unsigned long lastDebounce = 0;
    int reading = digitalRead(button);

    if (reading != lastButtonState)
    {
        lastDebounce = millis();
    }
    if (millis() - lastDebounce > 30)
    {
        buttonState = reading;
    }
    lastButtonState = reading;

    xReading = analogRead(xaxis);
    yReading = analogRead(yaxis);

    // if the reading is within the neutral drift range we treat it as zero if not we map it to (-5,5)

    // for x_axis movement
    if (xReading < minXValue || xReading > maxXValue)
    {
        if (xReading < minXValue)
        {
            xValue = map(xReading, 0, minXValue, -5, 0);
        }
        else
        {
            xValue = map(xReading, maxXValue, 1023, 0, 5);
        }
    }
    else
    {
        xValue = 0;
    }
    // for y_axis movement
    if (yReading < minYValue || yReading > maxYValue)
    {
        if (yReading < minYValue)
        {
            yValue = map(yReading, 0, minYValue, -5, 0);
        }
        else
        {
            yValue = map(yReading, maxYValue, 1023, 0, 5);
        }
    }
    else
    {
        yValue = 0;
    }

    // acceleration
    if (xValue == 5 || xValue == -5)
    {
        xCount++;
        if (xCount > count)
        {
            xScale++;
            xCount = 0;
        }
    }
    else
    {
        xScale = 1;
    }

    if (yValue == 5 || yValue == -5)
    {
        yCount++;
        if (yCount > count)
        {
            yScale++;
            yCount = 0;
        }
    }
    else
    {
        yScale = 1;
    }

    // limit scale
    xScale = constrain(xScale, 1, 10);
    yScale = constrain(yScale, 1, 10);

    // serial output
    Serial.print(xValue * xScale);
    Serial.print(",");
    Serial.print(yValue * yScale);
    Serial.print(",");
    Serial.println(buttonState);
    // small delay
    delay(5);
}
