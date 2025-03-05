
void Reload()
{
	ReadConfig();

	//AddOrRemoveCommands(false);

	//AddOrRemoveCommands();
}

void ReloadConfig(APlayerController* pc, FString*, bool)
{
	auto* spc = static_cast<AShooterPlayerController*>(pc);

	try
	{
		Reload();
	}
	catch (const std::exception& error)
	{
		AsaApi::GetApiUtils().SendServerMessage(spc, FColorList::Red, "Failed to reload config. ERROR: {}", error.what());
	}

	AsaApi::GetApiUtils().SendServerMessage(spc, FColorList::Green, "Config Reloaded.");
}

void ReloadConfigRcon(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld*)
{
	FString reply;

	try
	{
		Reload();
	}
	catch (const std::exception& error)
	{
		reply = error.what();
		rcon_connection->SendMessageW(rcon_packet->Id, 0, &reply);
		return;
	}

	reply = "Config Reloaded.";
	rcon_connection->SendMessageW(rcon_packet->Id, 0, &reply);
}
 
void AddReloadCommands(bool addCmd = true)
{
	if (addCmd)
	{
		AsaApi::GetCommands().AddConsoleCommand("PluginTemplate.Reload", &ReloadConfig);
		AsaApi::GetCommands().AddRconCommand("PluginTemplate.Reload", &ReloadConfigRcon);
	}
	else
	{
		AsaApi::GetCommands().RemoveConsoleCommand("PluginTemplate.Reload");
		AsaApi::GetCommands().RemoveRconCommand("PluginTemplate.Reload");
	}
}