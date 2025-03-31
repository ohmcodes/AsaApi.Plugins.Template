
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
	FString reloadCmd = std::string(PROJECT_NAME + std::string(".Reload")).c_str();

	if (addCmd)
	{
		AsaApi::GetCommands().AddConsoleCommand(reloadCmd, &ReloadConfig);
		AsaApi::GetCommands().AddRconCommand(reloadCmd, &ReloadConfigRcon);
	}
	else
	{
		AsaApi::GetCommands().RemoveConsoleCommand(reloadCmd);
		AsaApi::GetCommands().RemoveRconCommand(reloadCmd);
	}
}