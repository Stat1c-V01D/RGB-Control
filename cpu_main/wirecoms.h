#pragma once
#include "defines.h"
#include <Wire.h>

void wire_receive(int bytes);
void wire_send();

void wireinit()
{
	Wire.begin(10);
	Wire.onReceive(wire_receive);
	Wire.onRequest(wire_send);
}

void wireupdate()
{
}

void wire_receive(int howmany)
{

}

void wire_send()
{

}