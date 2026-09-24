#include "connection.h"
#include "display_functions.h"
#include "sound_functions.h"
#include "buttons_functions.h"



void setup() {
  // put your setup code here, to run once:

  initDisplay();

  displayTextMessage("Status: OK");
  displayColorFigure(0, 255, 0);


}

void loop() {
  // put your main code here, to run repeatedly:

}
