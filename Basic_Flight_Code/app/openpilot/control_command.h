#ifndef CONTROL_COMMAND_H
#define CONTROL_COMMAND_H
#include <core_common.h>
#include "data_common.h"
#include "param_common.h"
#include <device_includes.h>
//void AttitudeInitialize();
void command_handler();

extern bool fly;
extern bool exfly;
extern bool sweep_mode;
#endif // CONTROL_COMMAND_H
