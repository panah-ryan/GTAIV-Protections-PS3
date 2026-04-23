#pragma once
#include "stdafx.h"
#include "caller.h"

extern Caller* call;

static int PRINT_STRING_WITH_LITERAL_STRING_NOW(const char* gxtName, const char* text, int timeMS, int color) { return call->call<int>("PRINT_STRING_WITH_LITERAL_STRING_NOW", gxtName, text, timeMS, color); }
static int GET_CURRENT_EPISODE() { return call->call<int>("GET_CURRENT_EPISODE"); }
static BOOL IS_THIS_MODEL_A_PED(uint32_t model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_PED", model_hash); }
static BOOL IS_THIS_MODEL_A_VEHICLE(uint32_t model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_VEHICLE", model_hash); }
static BOOL IS_THIS_MODEL_A_CAR(uint32_t model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_CAR", model_hash); }
static BOOL IS_THIS_MODEL_A_BIKE(uint32_t model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_BIKE", model_hash); }
static BOOL IS_THIS_MODEL_A_BOAT(uint32_t model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_BOAT", model_hash); }
static BOOL IS_THIS_MODEL_A_HELI(uint32_t model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_HELI", model_hash); }
static BOOL IS_THIS_MODEL_A_TRAIN(uint32_t model_hash) { return call->call<BOOL>("IS_THIS_MODEL_A_TRAIN", model_hash); }