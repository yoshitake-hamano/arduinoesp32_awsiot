#pragma once

const int HIGH_INTERVAL_MS   = 250;
const int NORMAL_INTERVAL_MS = 500;
const int LOW_INTERVAL_MS    = 60000;

void initializeSystem(const char *title);
void restartSystem();
void turnOnActiveLed();
void turnOffActiveLed();
