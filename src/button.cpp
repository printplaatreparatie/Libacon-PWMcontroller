#include "LIMACH.h"

void Button(void)
{
    static bool MA9136State = false; // give the MA9136 the initial state of OFF

    if (!(digitalRead(button))) // if the button is LOW(pushed)
    {
        MA9136State = !MA9136State;        // update the state to new value
        digitalWrite(MA9136, MA9136State); // write the updated state to MA9136
    }
}
