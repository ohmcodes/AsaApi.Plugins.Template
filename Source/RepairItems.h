
void RepairItemsCallback(AShooterPlayerController* pc, FString* param, int, int)
{
	Log::GetLog()->warn("Function: {}", __FUNCTION__);

	// permissions

	// points checking

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

	
	AddPlayer(pc->GetEOSId(), pc->GetLinkedPlayerID(), pc->GetCharacterName());

	// points deductions


	if (affectedItemsCounter > 0)
	{
		AsaApi::GetApiUtils().SendNotification(pc, FColorList::Green, 1.3f, 15.0f, nullptr, "All items has been repaired.");
	}
}