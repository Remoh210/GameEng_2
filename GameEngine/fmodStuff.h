#pragma once
#include "fmod.hpp"
#include <fmod_errors.h>
#include <assert.h>




#define NUM_OF_SOUNDS 12
#define NUM_OF_CHANNEL_GROUPS 5
//FMOD
extern FMOD_RESULT _result;
extern FMOD::System *_system;
extern FMOD::Sound *_sound[NUM_OF_SOUNDS];
extern FMOD::Channel *_channel[NUM_OF_SOUNDS];
extern FMOD::ChannelGroup *_channel_groups[NUM_OF_CHANNEL_GROUPS];
extern FMOD::DSP *_dsp_echo;

extern FMOD_VECTOR _channel_position1;
extern FMOD_VECTOR _channel_position2;
extern FMOD_VECTOR _channel_position3;
extern FMOD_VECTOR _channel_velocity;
extern FMOD_VECTOR _listener_position;
extern FMOD_VECTOR _forward;
extern FMOD_VECTOR _up;


extern unsigned int _channel_position;
extern unsigned int _sound_lenght;
extern float _channel_frequency;
extern float _channel_volume;
extern float _channel_pan;
extern char _songname[128];
extern float _channel_pitch;
