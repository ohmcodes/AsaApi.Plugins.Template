
void RepairItemsCallback(AShooterPlayerController* pc, FString* param, int, int)
{
	Log::GetLog()->warn("Function: {}", __FUNCTION__);

	// permissions check
	FString perms = GetPriorPermByEOSID(pc->GetEOSId());
	nlohmann::json command = GetCommandString(perms.ToString(), "RepairItemCMD");

	if (command.is_null() || (!command.is_null() && command.value("Enabled", false) == false))
	{
		if (PluginTemplate::config["Debug"].value("Permissions", false) == true)
		{
			Log::GetLog()->info("{} No permissions. Command: {}", pc->GetCharacterName().ToString(), __FUNCTION__);
		}

		AsaApi::GetApiUtils().SendNotification(pc, FColorList::Red, 1.3f, 15.0f, nullptr, PluginTemplate::config["Messages"].value("RepairItemsPermErrorMSG", "You don't have permission to use this command.").c_str());

		return;
	}
	 
	// points checking
	if (Points(pc->GetEOSId(), command.value("Cost", 0), true) == false)
	{
		if (PluginTemplate::config["Debug"].value("Points", false) == true)
		{
			Log::GetLog()->info("{} don't have points. Command: {}", pc->GetCharacterName().ToString(), __FUNCTION__);
		}

		AsaApi::GetApiUtils().SendNotification(pc, FColorList::Red, 1.3f, 15.0f, nullptr, PluginTemplate::config["Messages"].value("PointsErrorMSG", "Not enough points.").c_str());

		return;
	}

	// execute
	ACharacter* character = pc->CharacterField().Get();
	if (!character) return;
	APrimalCharacter* primalCharacter = static_cast<APrimalCharacter*>(character);

	UPrimalInventoryComponent* invComp = primalCharacter->MyInventoryComponentField();
	if (!invComp) return;

	TArray<UPrimalItem*> playerInv = invComp->InventoryItemsField();

	bool ignoreMaterials = PluginTemplate::config["General"]["IgnoreInvRepairRequirements"].get<bool>();

	int affectedItemsCounter = 0;
	for (UPrimalItem* item : playerInv)
	{
		if (item->bIsEngram().Get()) continue;

		if (item->IsItemSkin(false)) continue;

		//if(!item->IsBroken()) only fix broken things

		if (!item->UsesDurability()) continue;

		if (item->ItemDurabilityField() == item->GetMaxDurability()) continue;

		if (ignoreMaterials)
		{
			item->ItemDurabilityField() = item->GetMaxDurability();
		}
		else
		{
			item->RepairItem(false, 1.0f, 1.0f);
		}

		item->UpdatedItem(false, false);
		affectedItemsCounter += 1;
	}

	if (ReadPlayer(pc->GetEOSId()))
	{
		UpdatePlayer(pc->GetEOSId(), pc->GetCharacterName());
	}
	else
	{
		AddPlayer(pc->GetEOSId(), pc->GetLinkedPlayerID(), pc->GetCharacterName());
	}

	// points deductions
	Points(pc->GetEOSId(), command.value("Cost", 0));

	if (affectedItemsCounter > 0)
	{
		AsaApi::GetApiUtils().SendNotification(pc, FColorList::Green, 1.3f, 15.0f, nullptr, PluginTemplate::config["Messages"].value("RepairItemsMSG", "All items has been repaired. {}").c_str(), pc->GetCharacterName().ToString());

		std::string msg = fmt::format("Player {} repaired their items. count {}", pc->GetCharacterName().ToString(), affectedItemsCounter);

		SendMessageToDiscord(msg);
	}
}


void DeletePlayerCallback(AShooterPlayerController* pc, FString* param, int, int)
{
	Log::GetLog()->warn("Function: {}", __FUNCTION__);


	//pc->GetEOSId(), pc->GetLinkedPlayerID(), pc->GetCharacterName()


	// permissions check
	FString perms = GetPriorPermByEOSID(pc->GetEOSId());
	nlohmann::json command = GetCommandString(perms.ToString(), "DeletePlayerCMD");

	if (command.is_null() || (!command.is_null() && command.value("Enabled", false) == false))
	{
		if (PluginTemplate::config["Debug"].value("Permissions", false) == true)
		{
			Log::GetLog()->info("{} No permissions. Command: {}", pc->GetCharacterName().ToString(), __FUNCTION__);
		}

		AsaApi::GetApiUtils().SendNotification(pc, FColorList::Red, 1.3f, 15.0f, nullptr, PluginTemplate::config["Messages"].value("RepairItemsPermErrorMSG", "You don't have permission to use this command.").c_str());

		return;
	}

	// points checking
	if (Points(pc->GetEOSId(), command.value("Cost", 0), true) == false)
	{
		if (PluginTemplate::config["Debug"].value("Points", false) == true)
		{
			Log::GetLog()->info("{} don't have points. Command: {}", pc->GetCharacterName().ToString(), __FUNCTION__);
		}

		AsaApi::GetApiUtils().SendNotification(pc, FColorList::Red, 1.3f, 15.0f, nullptr, PluginTemplate::config["Messages"].value("PointsErrorMSG", "Not enough points.").c_str());

		return;
	}


	DeletePlayer(pc->GetEOSId());

	// points deductions
	Points(pc->GetEOSId(), command.value("Cost", 0));

	AsaApi::GetApiUtils().SendNotification(pc, FColorList::Orange, 1.3f, 15.0f, nullptr, "Player deleted");
}