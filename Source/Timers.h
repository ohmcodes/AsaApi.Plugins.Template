
void TimerCallback()
{
	if (PluginTemplate::counter == 20)
	{
		Log::GetLog()->info("Server is up!");
	}

	if (PluginTemplate::counter == 50)
	{
		AsaApi::GetApiUtils().SendNotificationToAll(FColorList::Blue, 1.3f, 15.0f, nullptr, "Hooray Welcome to the server! enjoy!");
	}


	PluginTemplate::counter++;
}


void SetTimers(bool addTmr = true)
{
	if (addTmr)
	{
		AsaApi::GetCommands().AddOnTimerCallback("PluginTemplateTimerTick", &TimerCallback);
	}
	else
	{
		AsaApi::GetCommands().RemoveOnTimerCallback("PluginTemplateTimerTick");
	}
}