



#define NUM_OF_SOUNDS 10
#define NUM_OF_CHANNEL_GROUPS 4

std::string inputFile = "assets/music/songlist.txt";
//FMOD
FMOD_RESULT _result = FMOD_OK;
FMOD::System *_system = NULL;
FMOD::Sound *_sound[NUM_OF_SOUNDS];
FMOD::Channel *_channel[NUM_OF_SOUNDS];
FMOD::ChannelGroup *_channel_groups[NUM_OF_CHANNEL_GROUPS];
FMOD::DSP *_dsp_echo;

FMOD_VECTOR _channel_position1;
FMOD_VECTOR _channel_position2;
FMOD_VECTOR _channel_velocity = { 0.0f, 0.0f, 0.0f };
FMOD_VECTOR _listener_position = { 0.0f, 0.0f, 0.0f };
FMOD_VECTOR _forward = { 0.0f, 0.0f, -1.0f };
FMOD_VECTOR _up = { 0.0f, 1.0f, 0.0f };






bool init_fmod() {

	//Create the main system object
	_result = FMOD::System_Create(&_system);
	//TODO: CHECK FOR FMOD ERRORS, IMPLEMENT YOUR OWN FUNCTION
	assert(!_result);
	//Initializes the system object, and the msound device. This has to be called at the start of the user's program
	_result = _system->init(512, FMOD_INIT_3D_RIGHTHANDED, NULL);
	assert(!_result);


	return true;
}

bool shutdown_fmod() {

	for (unsigned int i = 0; i < NUM_OF_SOUNDS; i++)
	{
		if (_sound[i]) {
			_result = _sound[i]->release();
			assert(!_result);
		}
	}


	if (_system) {
		_result = _system->close();
		assert(!_result);
		_result = _system->release();
		assert(!_result);
	}

	return true;
}



void LoadFromFile()
{
	std::string files[NUM_OF_SOUNDS];
	int count = 0;

	std::ifstream inputfile;
	inputfile.open(inputFile);
	if (!inputfile.is_open())			// More "c" or "C++" ish
	{
		std::cout << "Didn't open file" << std::endl;
	}
	while (!inputfile.eof())
	{

		inputfile >> files[count];
		std::cout << files[count] << std::endl;
		if (count < 3) {
			_result = _system->createSound(files[count].c_str(), FMOD_3D, 0, &_sound[count]);
			assert(!_result);
		}
		else
		{
			_system->createStream(files[count].c_str(), FMOD_DEFAULT, 0, &_sound[count]);
			assert(!_result);
		}
		count++;
	}

}


void SetUpSound()
{
	//Master Group
	_result = _system->getMasterChannelGroup(&_channel_groups[0]);
	assert(!_result);

	//
	_result = _system->createChannelGroup("Group 3D Sound", &_channel_groups[1]);
	assert(!_result);
	_result = _system->createChannelGroup("Music Group 1", &_channel_groups[2]);
	assert(!_result);
	_result = _system->createChannelGroup("Music Group 2", &_channel_groups[3]);
	assert(!_result);
	_result = _system->createChannelGroup("Music Group 2", &_channel_groups[4]);
	assert(!_result);

	//Set groups children of master group.
	_result = _channel_groups[0]->addGroup(_channel_groups[1]);
	assert(!_result);
	_result = _channel_groups[0]->addGroup(_channel_groups[2]);
	assert(!_result);
	_result = _channel_groups[0]->addGroup(_channel_groups[3]);
	assert(!_result);
	_result = _channel_groups[0]->addGroup(_channel_groups[4]);
	assert(!_result);


	//3d Sound 
	_result = _sound[0]->set3DMinMaxDistance(100.0f, 10000.0f);
	assert(!_result);
	_result = _sound[0]->setMode(FMOD_LOOP_NORMAL);
	assert(!_result);
	_result = _system->playSound(_sound[0], _channel_groups[0], false, &_channel[0]);
	assert(!_result);
	_channel_position1 = { 0.0f,0.0f,0.0f };
	_result = _channel[0]->set3DAttributes(&_channel_position1, &_channel_velocity);
	assert(!_result);

	//Streaming Sounds

	//_result = _system->playSound(_sound[3], _channel_groups[1], false, &_channel[4]);
	//assert(!_result);

	//TODO7: Create dsp echo
	//_result = _system->createDSPByType(FMOD_DSP_TYPE_ECHO, &_dsp_echo);
	//assert(!_result);

	//_result = _channel_groups[1]->addDSP(0, _dsp_echo);
	//assert(!_result);
	//_result = _dsp_echo->setBypass(true);
	//assert(!_result);
}

void UpdateSound() {
	//listener Position
	_listener_position = { g_CameraEye.x, g_CameraEye.y, g_CameraEye.z };
	//glm::vec3 cameraF = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	//cameraFront =

	glm::vec3 horizon = glm::normalize(glm::cross(cameraFront, cameraUp));
	glm::vec3 newup = glm::normalize(glm::cross(horizon, cameraFront));
	_up = { newup.x, newup.y, newup.z };
	_forward = { cameraFront.x, cameraFront.y, cameraFront.z };
	_result = _system->set3DListenerAttributes(0, &_listener_position, &_channel_velocity, &_forward, &_up);
	assert(!_result);

	//Sound 1 position
	cMeshObject* bonfire = findObjectByFriendlyName("bonfire");
	_channel_position1 = { bonfire->position.x, bonfire->position.y, bonfire->position.z };
	_result = _channel[0]->set3DAttributes(&_channel_position1, &_channel_velocity);

	_result = _system->update();
	assert(!_result);
}