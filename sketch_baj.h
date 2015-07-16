#ifndef SKETCH_BAJ_H
#define SKETCH_BAJ_H

typedef enum {
  MODE_ROTATE_TEST,
  MODE_COMPASS,
  MODE_ASCII
} Mode;

typedef struct {
  int x;
  int y;
  int z;
} Triple;

typedef struct {
  float x;
  float y;
} Tuple;

// Timer callbacks.
void toggleLedActivity();
void updatePololuLedStrip();

// Inertial module.
void setupInertialModule();
unsigned int handleInertialModuleCompass();

#endif // SKETCH_BAJ_H
