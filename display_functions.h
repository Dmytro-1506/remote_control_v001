#pragma once

#include <Arduino.h>

void initDisplay();

void displayTextMessage(const String &message);
void displayColorFigure(int r, int g, int b);
