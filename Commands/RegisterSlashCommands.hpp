// RegisterSlashCommands.hpp - Registers the slash commands of this bot.
// May 27, 2021
// Chris M.
// https://github.com/RealTimeChris

#pragma once

#ifndef _REGISTER_SLASH_COMMANDS_
#define _REGISTER_SLASH_COMMANDS_

#include "../pch.h"
#include "../DiscordCoreClient.hpp"
#include "Commands.hpp"

namespace DiscordCoreAPI {
	
	class RegisterSlashCommands :public BaseFunction {
	public:
		RegisterSlashCommands() {
			this->commandName = "registerslashcommands";
			this->helpDescription = "__**Register Slash Commands Usage:**__ !registerslashcommands or /registerslashcommands!";
		}
		virtual task<void> execute(shared_ptr<BaseFunctionArguments> args) {
			/*
			CreateApplicationCommandData createSellDrugsCommandData;
			createSellDrugsCommandData.defaultPermission = true;
			createSellDrugsCommandData.description = "Sell drugs in exchange for some currency!";
			createSellDrugsCommandData.name = "selldrugs";
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createSellDrugsCommandData);
			
			CreateApplicationCommandData registerSlashCommandsCommandData;
			registerSlashCommandsCommandData.defaultPermission = true;
			registerSlashCommandsCommandData.description = "Register the programmatically designated slash commands.";
			registerSlashCommandsCommandData.name = "registerslashcommands";
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(registerSlashCommandsCommandData);
			
			CreateApplicationCommandData createBalanceCommandData;
			createBalanceCommandData.defaultPermission = true;
			createBalanceCommandData.description = "Check your or another person's currency balances.";
			createBalanceCommandData.name = "balance";
			ApplicationCommandOptionData applicationCommandOptionOne;
			applicationCommandOptionOne.name = "person";
			applicationCommandOptionOne.required = false;
			applicationCommandOptionOne.type = ApplicationCommandOptionType::USER;
			applicationCommandOptionOne.description = "The person who's balances you would like to check.";
			createBalanceCommandData.options.push_back(applicationCommandOptionOne);
			args->coreClient->slashCommands->createGlobalApplicationCommandAsync(createBalanceCommandData);

			CreateApplicationCommandData createAddShopItemCommandData;
			createAddShopItemCommandData.defaultPermission = true;
			createAddShopItemCommandData.description = "Add an item to the shop's inventory.";
			createAddShopItemCommandData.name = "addshopitem";
			ApplicationCommandOptionData addShopItemCommandOptionOne;
			addShopItemCommandOptionOne.name = "itemname";
			addShopItemCommandOptionOne.required = true;
			addShopItemCommandOptionOne.type = ApplicationCommandOptionType::STRING;
			addShopItemCommandOptionOne.description = "The name of the item.";
			createAddShopItemCommandData.options.push_back(addShopItemCommandOptionOne);
			ApplicationCommandOptionData addShopItemCommandOptionTwo;
			addShopItemCommandOptionTwo.name = "selfmod";
			addShopItemCommandOptionTwo.required = true;
			addShopItemCommandOptionTwo.type = ApplicationCommandOptionType::INTEGER;
			addShopItemCommandOptionTwo.description = "The self-mod value of the item.";
			createAddShopItemCommandData.options.push_back(addShopItemCommandOptionTwo);
			ApplicationCommandOptionData addShopItemCommandOptionThree;
			addShopItemCommandOptionThree.name = "oppmod";
			addShopItemCommandOptionThree.required = true;
			addShopItemCommandOptionThree.type = ApplicationCommandOptionType::INTEGER;
			addShopItemCommandOptionThree.description = "The opp-mod value of the item.";
			createAddShopItemCommandData.options.push_back(addShopItemCommandOptionThree);
			ApplicationCommandOptionData addShopItemCommandOptionFour;
			addShopItemCommandOptionFour.name = "itemcost";
			addShopItemCommandOptionFour.required = true;
			addShopItemCommandOptionFour.type = ApplicationCommandOptionType::INTEGER;
			addShopItemCommandOptionFour.description = "The value/cost of the item.";
			createAddShopItemCommandData.options.push_back(addShopItemCommandOptionFour);
			ApplicationCommandOptionData addShopItemCommandOptionFive;
			addShopItemCommandOptionFive.name = "emoji";
			addShopItemCommandOptionFive.required = true;
			addShopItemCommandOptionFive.type = ApplicationCommandOptionType::STRING;
			addShopItemCommandOptionFive.description = "The emoji/icon to use for the item.";
			createAddShopItemCommandData.options.push_back(addShopItemCommandOptionFive);
			args->coreClient->slashCommands->createGlobalApplicationCommandAsync(createAddShopItemCommandData);

			CreateApplicationCommandData createAddShopRoleCommandData;
			createAddShopRoleCommandData.defaultPermission = true;
			createAddShopRoleCommandData.description = "Add a role to the shop's inventory.";
			createAddShopRoleCommandData.name = "addshoprole";
			ApplicationCommandOptionData addShopRoleCommandOptionOne;
			addShopRoleCommandOptionOne.name = "rolename";
			addShopRoleCommandOptionOne.required = true;
			addShopRoleCommandOptionOne.type = ApplicationCommandOptionType::STRING;
			addShopRoleCommandOptionOne.description = "The name of the role.";
			createAddShopRoleCommandData.options.push_back(addShopRoleCommandOptionOne);
			ApplicationCommandOptionData addShopRoleCommandOptionTwo;
			addShopRoleCommandOptionTwo.name = "hexcolorvalue";
			addShopRoleCommandOptionTwo.required = true;
			addShopRoleCommandOptionTwo.type = ApplicationCommandOptionType::STRING;
			addShopRoleCommandOptionTwo.description = "The hex-color value of the role.";
			createAddShopRoleCommandData.options.push_back(addShopRoleCommandOptionTwo);
			ApplicationCommandOptionData addShopRoleCommandOptionThree;
			addShopRoleCommandOptionThree.name = "rolecost";
			addShopRoleCommandOptionThree.required = true;
			addShopRoleCommandOptionThree.type = ApplicationCommandOptionType::INTEGER;
			addShopRoleCommandOptionThree.description = "The value/cost of the role.";
			createAddShopRoleCommandData.options.push_back(addShopRoleCommandOptionThree);
			args->coreClient->slashCommands->createGlobalApplicationCommandAsync(createAddShopRoleCommandData);

			CreateApplicationCommandData createCoinflipRoleCommandData;
			createCoinflipRoleCommandData.defaultPermission = true;
			createCoinflipRoleCommandData.description = "Play heads or tails.";
			createCoinflipRoleCommandData.name = "coinflip";
			ApplicationCommandOptionData coinflipCommandOptionOne;
			coinflipCommandOptionOne.name = "betamount";
			coinflipCommandOptionOne.required = true;
			coinflipCommandOptionOne.type = ApplicationCommandOptionType::INTEGER;
			coinflipCommandOptionOne.description = "The wager you would like to place.";
			createCoinflipRoleCommandData.options.push_back(coinflipCommandOptionOne);
			args->coreClient->slashCommands->createGlobalApplicationCommandAsync(createCoinflipRoleCommandData);

			CreateApplicationCommandData createBotInfoCommandData;
			createBotInfoCommandData.defaultPermission = true;
			createBotInfoCommandData.description = "Display info about the current bot.";
			createBotInfoCommandData.name = "botinfo";
			ApplicationCommandOptionData botInfoCommandOptionOne;
			botInfoCommandOptionOne.name = "botname";
			botInfoCommandOptionOne.required = true;
			botInfoCommandOptionOne.type = ApplicationCommandOptionType::STRING;
			botInfoCommandOptionOne.description = "The bot which you would like to display the info of.";
			ApplicationCommandOptionChoiceData botInfoChoiceOne;
			botInfoChoiceOne.name = "name";
			botInfoChoiceOne.valueString = "gamehouse";
			botInfoCommandOptionOne.choices.push_back(botInfoChoiceOne);
			createBotInfoCommandData.options.push_back(botInfoCommandOptionOne);
			args->coreClient->slashCommands->createGlobalApplicationCommandAsync(createBotInfoCommandData);

			CreateApplicationCommandData createBlackjackCommandData;
			createBlackjackCommandData.defaultPermission = true;
			createBlackjackCommandData.description = "Play a round of blackjack.";
			createBlackjackCommandData.name = "blackjack";
			ApplicationCommandOptionData blackJackCommandOptionOne;
			blackJackCommandOptionOne.name = "betamount";
			blackJackCommandOptionOne.required = true;
			blackJackCommandOptionOne.type = ApplicationCommandOptionType::INTEGER;
			blackJackCommandOptionOne.description = "The amount which you would like to wager.";
			createBlackjackCommandData.options.push_back(blackJackCommandOptionOne);
			args->coreClient->slashCommands->createGlobalApplicationCommandAsync(createBlackjackCommandData);

			CreateApplicationCommandData createButtonsCommandData;
			createButtonsCommandData.defaultPermission = true;
			createButtonsCommandData.description = "Test the buttons.";
			createButtonsCommandData.name = "buttons";
			args->coreClient->slashCommands->createGlobalApplicationCommandAsync(createButtonsCommandData);
			
			CreateApplicationCommandData createShopCommandData;
			createShopCommandData.defaultPermission = true;
			createShopCommandData.description = "Check out the server's shop!";
			createShopCommandData.name = "shop";
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createShopCommandData);
			
			CreateApplicationCommandData createBuyCommandData;
			createBuyCommandData.defaultPermission = true;
			createBuyCommandData.description = "Purchase an item from the guildshop.";
			createBuyCommandData.name = "buy";
			ApplicationCommandOptionData buyCommandOptionOne;
			buyCommandOptionOne.name = "objectname";
			buyCommandOptionOne.required = true;
			buyCommandOptionOne.type = ApplicationCommandOptionType::STRING;
			buyCommandOptionOne.description = "The item or role which you would like to purchase.";
			createBuyCommandData.options.push_back(buyCommandOptionOne);
			args->coreClient->slashCommands->createGlobalApplicationCommandAsync(createBuyCommandData);
			
			DiscordCoreAPI::CreateApplicationCommandData createTestCommandData;
			createTestCommandData.defaultPermission = true;
			createTestCommandData.description = "A test command.";
			createTestCommandData.name = "test";
			DiscordCoreAPI::ApplicationCommandOptionData testCommandOptionOne;
			testCommandOptionOne.name = "optionone";
			testCommandOptionOne.required = true;
			testCommandOptionOne.type = ApplicationCommandOptionType::STRING;
			testCommandOptionOne.description = "The first argument to be entered.";
			createTestCommandData.options.push_back(testCommandOptionOne);
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createTestCommandData);
			
			DiscordCoreAPI::CreateApplicationCommandData createBotInfoCommandData;
			createBotInfoCommandData.defaultPermission = true;
			createBotInfoCommandData.description = "Displays info about the current bot.";
			createBotInfoCommandData.name = "botinfo";
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createBotInfoCommandData);
			
			DiscordCoreAPI::CreateApplicationCommandData createCasinoStatsCommandData;
			createCasinoStatsCommandData.defaultPermission = true;
			createCasinoStatsCommandData.description = "View the server's casino stats.";
			createCasinoStatsCommandData.name = "casinostats";
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createCasinoStatsCommandData);
			
			DiscordCoreAPI::CreateApplicationCommandData createDisplayGuildsDataCommandData;
			createDisplayGuildsDataCommandData.defaultPermission = true;
			createDisplayGuildsDataCommandData.description = "View the list of servers that this bot is in.";
			createDisplayGuildsDataCommandData.name = "displayguildsdata";
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createDisplayGuildsDataCommandData);
			
			DiscordCoreAPI::CreateApplicationCommandData createDuelCommandData;
			createDuelCommandData.defaultPermission = true;
			createDuelCommandData.description = "Challenge another server member do a duel.";
			createDuelCommandData.name = "duel";
			DiscordCoreAPI::ApplicationCommandOptionData duelCommandOptionOne;
			duelCommandOptionOne.name = "challengetarget";
			duelCommandOptionOne.required = true;
			duelCommandOptionOne.type = ApplicationCommandOptionType::USER;
			duelCommandOptionOne.description = "The individual you are challenging to a duel.";
			createDuelCommandData.options.push_back(duelCommandOptionOne);
			DiscordCoreAPI::ApplicationCommandOptionData duelCommandOptionTwo;
			duelCommandOptionTwo.name = "betamount";
			duelCommandOptionTwo.required = true;
			duelCommandOptionTwo.type = ApplicationCommandOptionType::INTEGER;
			duelCommandOptionTwo.description = "The amount of the wager you are putting up.";
			createDuelCommandData.options.push_back(duelCommandOptionTwo);
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createDuelCommandData);
			
			DiscordCoreAPI::CreateApplicationCommandData createGamehouseOptionsCommandData;
			createGamehouseOptionsCommandData.defaultPermission = true;
			createGamehouseOptionsCommandData.description = "View the options of this bot.";
			createGamehouseOptionsCommandData.name = "gamehouseoptions";
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createGamehouseOptionsCommandData).get();
			
			DiscordCoreAPI::CreateApplicationCommandData createInventoryCommandData;
			createInventoryCommandData.defaultPermission = true;
			createInventoryCommandData.description = "Observe your inventory of items and roles.";
			createInventoryCommandData.name = "inventory";
			DiscordCoreAPI::ApplicationCommandOptionData inventoryCommandOptionOne;
			inventoryCommandOptionOne.name = "user";
			inventoryCommandOptionOne.required = false;
			inventoryCommandOptionOne.type = ApplicationCommandOptionType::USER;
			inventoryCommandOptionOne.description = "The user who's inventory you would like to view.";
			createInventoryCommandData.options.push_back(inventoryCommandOptionOne);
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createInventoryCommandData).get();
			
			DiscordCoreAPI::CreateApplicationCommandData createLeaderboardCommandData;
			createLeaderboardCommandData.defaultPermission = true;
			createLeaderboardCommandData.description = "View the server's currency leaderboard.";
			createLeaderboardCommandData.name = "leaderboard";
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createLeaderboardCommandData).get();			

			DiscordCoreAPI::CreateApplicationCommandData createRemoveObjectCommandData;
			createRemoveObjectCommandData.defaultPermission = true;
			createRemoveObjectCommandData.description = "Remove an object or role from a user's inventory.";
			createRemoveObjectCommandData.name = "removeobject";
			DiscordCoreAPI::ApplicationCommandOptionData testCommandOptionOne;
			testCommandOptionOne.name = "objectname";
			testCommandOptionOne.required = false;
			testCommandOptionOne.type = ApplicationCommandOptionType::STRING;
			testCommandOptionOne.description = "The object to be removed.";
			createRemoveObjectCommandData.options.push_back(testCommandOptionOne);
			DiscordCoreAPI::ApplicationCommandOptionData testCommandOptionTwo;
			testCommandOptionTwo.name = "role";
			testCommandOptionTwo.required = false;
			testCommandOptionTwo.type = ApplicationCommandOptionType::ROLE;
			testCommandOptionTwo.description = "The role to be removed.";
			createRemoveObjectCommandData.options.push_back(testCommandOptionTwo);
			DiscordCoreAPI::ApplicationCommandOptionData testCommandOptionThree;
			testCommandOptionThree.name = "user";
			testCommandOptionThree.required = false;
			testCommandOptionThree.type = ApplicationCommandOptionType::USER;
			testCommandOptionThree.description = "The user to remove the object/role from.";
			createRemoveObjectCommandData.options.push_back(testCommandOptionThree);
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createRemoveObjectCommandData);
			
			DiscordCoreAPI::CreateApplicationCommandData createRemoveShopItemCommandData;
			createRemoveShopItemCommandData.defaultPermission = true;
			createRemoveShopItemCommandData.description = "Remove an item from the server's shop.";
			createRemoveShopItemCommandData.name = "removeshopitem";
			DiscordCoreAPI::ApplicationCommandOptionData removeShopItemOptionOne;
			removeShopItemOptionOne.name = "item";
			removeShopItemOptionOne.required = true;
			removeShopItemOptionOne.type = ApplicationCommandOptionType::STRING;
			removeShopItemOptionOne.description = "The item which you would like to remove.";
			createRemoveShopItemCommandData.options.push_back(removeShopItemOptionOne);
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createRemoveShopItemCommandData).get();

			DiscordCoreAPI::CreateApplicationCommandData createRemoveShopRoleCommandData;
			createRemoveShopRoleCommandData.defaultPermission = true;
			createRemoveShopRoleCommandData.description = "Remove a role from the server's shop.";
			createRemoveShopRoleCommandData.name = "removeshoprole";
			DiscordCoreAPI::ApplicationCommandOptionData removeShopRoleOptionOne;
			removeShopRoleOptionOne.name = "role";
			removeShopRoleOptionOne.required = true;
			removeShopRoleOptionOne.type = ApplicationCommandOptionType::ROLE;
			removeShopRoleOptionOne.description = "The role which you would like to remove.";
			createRemoveShopRoleCommandData.options.push_back(removeShopRoleOptionOne);
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createRemoveShopRoleCommandData).get();
			
			DiscordCoreAPI::CreateApplicationCommandData createRobCommandData;
			createRobCommandData.defaultPermission = true;
			createRobCommandData.description = "Rob a user for currency.";
			createRobCommandData.name = "rob";
			DiscordCoreAPI::ApplicationCommandOptionData robOptionOne;
			robOptionOne.name = "user";
			robOptionOne.required = true;
			robOptionOne.type = ApplicationCommandOptionType::USER;
			robOptionOne.description = "The user which you would like to rob.";
			createRobCommandData.options.push_back(robOptionOne);
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createRobCommandData).get();

			DiscordCoreAPI::CreateApplicationCommandData createRouletteCommandData;
			createRouletteCommandData.defaultPermission = true;
			createRouletteCommandData.description = "Start or bet on a game of roulette.";
			createRouletteCommandData.name = "roulette";
			DiscordCoreAPI::ApplicationCommandOptionData rouletteOptionOne;
			rouletteOptionOne.name = "start";
			rouletteOptionOne.type = ApplicationCommandOptionType::SUB_COMMAND;
			rouletteOptionOne.description = "Begins the roulette round.";
			DiscordCoreAPI::ApplicationCommandOptionData rouletteOptionOneOne;
			rouletteOptionOneOne.name = "start";
			rouletteOptionOneOne.type = ApplicationCommandOptionType::STRING;
			rouletteOptionOneOne.description = "Begin the game of roulette.";
			rouletteOptionOneOne.required = true;
			ApplicationCommandOptionChoiceData choiceOne;
			choiceOne.name = "start";
			choiceOne.valueString = "start";
			rouletteOptionOneOne.choices.push_back(choiceOne);
			rouletteOptionOne.options.push_back(rouletteOptionOneOne);
			createRouletteCommandData.options.push_back(rouletteOptionOne);
			DiscordCoreAPI::ApplicationCommandOptionData rouletteOptionTwo;
			rouletteOptionTwo.name = "bet1";
			rouletteOptionTwo.type = ApplicationCommandOptionType::SUB_COMMAND;
	 		rouletteOptionTwo.description = "Bet on an active game of roulette.";
			DiscordCoreAPI::ApplicationCommandOptionData rouletteOptionTwoNegOne;
			rouletteOptionTwoNegOne.name = "bet";
			rouletteOptionTwoNegOne.type = ApplicationCommandOptionType::STRING;
			rouletteOptionTwoNegOne.description = "Bet on the game of roulette.";
			rouletteOptionTwoNegOne.required = true;
			ApplicationCommandOptionChoiceData choiceTwoOne;
			choiceTwoOne.name = "bet";
			choiceTwoOne.valueString = "bet";
			rouletteOptionTwoNegOne.choices.push_back(choiceTwoOne);
			rouletteOptionTwo.options.push_back(rouletteOptionTwoNegOne);
			ApplicationCommandOptionData rouletteOptionTwoOh;
			rouletteOptionTwoOh.name = "amount";
			rouletteOptionTwoOh.description = "Enter the amount which you would like to bet";
			rouletteOptionTwoOh.required = true;
			rouletteOptionTwoOh.type = ApplicationCommandOptionType::INTEGER;
			rouletteOptionTwo.options.push_back(rouletteOptionTwoOh);
			DiscordCoreAPI::ApplicationCommandOptionData rouletteOptionTwoOne;
			rouletteOptionTwoOne.name = "type";
			rouletteOptionTwoOne.type = ApplicationCommandOptionType::STRING;
			rouletteOptionTwoOne.required = true;
			rouletteOptionTwoOne.description = "Choose the type of bet you would like to make.";
			ApplicationCommandOptionChoiceData choice01;
			choice01.name = "0";
			choice01.valueString = "0";
			rouletteOptionTwoOne.choices.push_back(choice01);
			ApplicationCommandOptionChoiceData choice02;
			choice02.name = "00";
			choice02.valueString = "00";
			rouletteOptionTwoOne.choices.push_back(choice02);
			ApplicationCommandOptionChoiceData choice03;
			choice03.name = "straight";
			choice03.valueString = "straight";
			rouletteOptionTwoOne.choices.push_back(choice03);
			ApplicationCommandOptionChoiceData choice04;
			choice04.name = "row";
			choice04.valueString = "row";
			rouletteOptionTwoOne.choices.push_back(choice04);
			ApplicationCommandOptionChoiceData choice05;
			choice05.name = "split";
			choice05.valueString = "split";
			rouletteOptionTwoOne.choices.push_back(choice05);
			ApplicationCommandOptionChoiceData choice06;
			choice06.name = "street";
			choice06.valueString = "street";
			rouletteOptionTwoOne.choices.push_back(choice06);
			ApplicationCommandOptionChoiceData choice07;
			choice07.name = "basket";
			choice07.valueString = "basket";
			rouletteOptionTwoOne.choices.push_back(choice07);
			ApplicationCommandOptionChoiceData choice08;
			choice07.name = "sixline";
			choice07.valueString = "sixline";
			rouletteOptionTwoOne.choices.push_back(choice08);
			ApplicationCommandOptionChoiceData choice09;
			choice09.name = "1stcolumn";
			choice09.valueString = "1stcolumn";
			rouletteOptionTwoOne.choices.push_back(choice09);
			ApplicationCommandOptionChoiceData choice10;
			choice10.name = "2ndcolumn";
			choice10.valueString = "2ndcolumn";
			rouletteOptionTwoOne.choices.push_back(choice10);
			rouletteOptionTwo.options.push_back(rouletteOptionTwoOne);
			ApplicationCommandOptionData rouletteOptionTwoTwo;
			rouletteOptionTwoTwo.name = "betoptions";
			rouletteOptionTwoTwo.description = "Bet options for select bet-types.";
			rouletteOptionTwoOne.required = false;
			rouletteOptionTwoTwo.type = ApplicationCommandOptionType::INTEGER;
			rouletteOptionTwo.options.push_back(rouletteOptionTwoTwo);
			createRouletteCommandData.options.push_back(rouletteOptionTwo);
			DiscordCoreAPI::ApplicationCommandOptionData rouletteOptionThree;
			rouletteOptionThree.name = "bet2";
			rouletteOptionThree.type = ApplicationCommandOptionType::SUB_COMMAND;
			rouletteOptionThree.description = "Bet on an active game of roulette.";
			DiscordCoreAPI::ApplicationCommandOptionData rouletteOptionThreeNegOne;
			rouletteOptionThreeNegOne.name = "bet";
			rouletteOptionThreeNegOne.type = ApplicationCommandOptionType::STRING;
			rouletteOptionThreeNegOne.description = "Bet on the game of roulette.";
			rouletteOptionThreeNegOne.required = true;
			ApplicationCommandOptionChoiceData choiceThreeOne;
			choiceThreeOne.name = "bet";
			choiceThreeOne.valueString = "bet";
			rouletteOptionThreeNegOne.choices.push_back(choiceThreeOne);
			rouletteOptionThree.options.push_back(rouletteOptionThreeNegOne);
			ApplicationCommandOptionData rouletteOptionThreeOh;
			rouletteOptionThreeOh.name = "amount";
			rouletteOptionThreeOh.description = "Enter the amount which you would like to bet";
			rouletteOptionThreeOh.required = true;
			rouletteOptionThreeOh.type = ApplicationCommandOptionType::INTEGER;
			rouletteOptionThree.options.push_back(rouletteOptionThreeOh);
			DiscordCoreAPI::ApplicationCommandOptionData rouletteOptionThreeOne;
			rouletteOptionThreeOne.name = "type";
			rouletteOptionThreeOne.type = ApplicationCommandOptionType::STRING;
			rouletteOptionThreeOne.required = true;
			rouletteOptionThreeOne.description = "Choose the type of bet you would like to make.";
			ApplicationCommandOptionChoiceData choice11;
			choice11.name = "3rdcolumn";
			choice11.valueString = "3rdcolumn";
			rouletteOptionThreeOne.choices.push_back(choice11);
			ApplicationCommandOptionChoiceData choice12;
			choice12.name = "1stdozen";
			choice12.valueString = "1stdozen";
			rouletteOptionThreeOne.choices.push_back(choice12);
			ApplicationCommandOptionChoiceData choice13;
			choice13.name = "2nddozen";
			choice13.valueString = "2nddozen";
			rouletteOptionThreeOne.choices.push_back(choice13);
			ApplicationCommandOptionChoiceData choice14;
			choice14.name = "3rddozen";
			choice14.valueString = "3rddozen";
			rouletteOptionThreeOne.choices.push_back(choice14);
			ApplicationCommandOptionChoiceData choice15;
			choice15.name = "odd";
			choice15.valueString = "odd";
			rouletteOptionThreeOne.choices.push_back(choice15);
			ApplicationCommandOptionChoiceData choice16;
			choice16.name = "even";
			choice16.valueString = "even";
			rouletteOptionThreeOne.choices.push_back(choice16);
			ApplicationCommandOptionChoiceData choice17;
			choice17.name = "red";
			choice17.valueString = "red";
			rouletteOptionThreeOne.choices.push_back(choice17);
			ApplicationCommandOptionChoiceData choice18;
			choice18.name = "black";
			choice18.valueString = "black";
			rouletteOptionThreeOne.choices.push_back(choice18);
			ApplicationCommandOptionChoiceData choice19;
			choice19.name = "1to18";
			choice19.valueString = "1to18";
			rouletteOptionThreeOne.choices.push_back(choice19);
			ApplicationCommandOptionChoiceData choice20;
			choice20.name = "19to36";
			choice20.valueString = "19to36";
			rouletteOptionThreeOne.choices.push_back(choice20);
			rouletteOptionThree.options.push_back(rouletteOptionThreeOne);
			ApplicationCommandOptionData rouletteOptionThreeTwo;
			rouletteOptionThreeTwo.name = "betoptions";
			rouletteOptionThreeTwo.description = "Bet options for select bet-types.";
			rouletteOptionThreeTwo.required = false;
			rouletteOptionThreeTwo.type = ApplicationCommandOptionType::INTEGER;
			rouletteOptionThree.options.push_back(rouletteOptionThreeTwo);
			createRouletteCommandData.options.push_back(rouletteOptionThree);
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createRouletteCommandData).get();
*/
			
DiscordCoreAPI::CreateApplicationCommandData createSetGameChannelCommandData;
			createSetGameChannelCommandData.defaultPermission = true;
			createSetGameChannelCommandData.description = "Sets the channels which you can play games in.";
			createSetGameChannelCommandData.name = "setgamechannel";
			DiscordCoreAPI::ApplicationCommandOptionData createSetGameChannelOptionOne;
			createSetGameChannelOptionOne.type = ApplicationCommandOptionType::SUB_COMMAND;
			createSetGameChannelOptionOne.name = "view";
			createSetGameChannelOptionOne.description = "View the currently enabled channels.";
			createSetGameChannelCommandData.options.push_back(createSetGameChannelOptionOne);
			DiscordCoreAPI::ApplicationCommandOptionData createSetGameChannelOptionTwo;
			createSetGameChannelOptionTwo.type = ApplicationCommandOptionType::SUB_COMMAND;
			createSetGameChannelOptionTwo.name = "add";
			createSetGameChannelOptionTwo.description = "Add a channel to the list of enabled channels.";
			ApplicationCommandOptionData createSetGameChannelOptionTwoOne;
			createSetGameChannelOptionTwoOne.name = "add";
			createSetGameChannelOptionTwoOne.description = "Add a channel to the list of enabled channels";
			createSetGameChannelOptionTwoOne.required = true;
			createSetGameChannelOptionTwoOne.type = ApplicationCommandOptionType::STRING;
			ApplicationCommandOptionChoiceData choiceOne;
			choiceOne.name = "add";
			choiceOne.valueString = "add";
			createSetGameChannelOptionTwoOne.choices.push_back(choiceOne);
			createSetGameChannelOptionTwo.options.push_back(createSetGameChannelOptionTwoOne);
			createSetGameChannelCommandData.options.push_back(createSetGameChannelOptionTwo);
			DiscordCoreAPI::ApplicationCommandOptionData createSetGameChannelOptionThree;
			createSetGameChannelOptionThree.type = ApplicationCommandOptionType::SUB_COMMAND;
			createSetGameChannelOptionThree.name = "remove";
			createSetGameChannelOptionThree.description = "Remove a channel to the list of enabled channels.";
			ApplicationCommandOptionData createSetGameChannelOptionThreeOne;
			createSetGameChannelOptionThreeOne.name = "remove";
			createSetGameChannelOptionThreeOne.description = "Remove a channel to the list of enabled channels";
			createSetGameChannelOptionThreeOne.required = true;
			createSetGameChannelOptionThreeOne.type = ApplicationCommandOptionType::STRING;
			ApplicationCommandOptionChoiceData choiceTwo;
			choiceTwo.name = "remove";
			choiceTwo.valueString = "remove";
			createSetGameChannelOptionThreeOne.choices.push_back(choiceTwo);
			createSetGameChannelOptionThree.options.push_back(createSetGameChannelOptionThreeOne);
			createSetGameChannelCommandData.options.push_back(createSetGameChannelOptionThree);
			DiscordCoreAPI::ApplicationCommandOptionData createSetGameChannelOptionFour;
			createSetGameChannelOptionFour.type = ApplicationCommandOptionType::SUB_COMMAND;
			createSetGameChannelOptionFour.name = "purge";
			createSetGameChannelOptionFour.description = "Purges the list of channels for the server.";
			ApplicationCommandOptionData createSetGameChannelOptionFourOne;
			createSetGameChannelOptionFourOne.name = "purge";
			createSetGameChannelOptionFourOne.description = "Purges the list of channels for the server.";
			createSetGameChannelOptionFourOne.required = true;
			createSetGameChannelOptionFourOne.type = ApplicationCommandOptionType::STRING;
			ApplicationCommandOptionChoiceData choiceThree;
			choiceThree.name = "purge";
			choiceThree.valueString = "purge";
			createSetGameChannelOptionFourOne.choices.push_back(choiceThree);
			createSetGameChannelOptionFour.options.push_back(createSetGameChannelOptionFourOne);
			createSetGameChannelCommandData.options.push_back(createSetGameChannelOptionFour);
			args->eventData.discordCoreClient->slashCommands->createGlobalApplicationCommandAsync(createSetGameChannelCommandData).get();
			InputEventManager::deleteInputEventResponse(args->eventData);
			
			Guild guild = args->eventData.discordCoreClient->guilds->getGuildAsync({ .guildId = args->eventData.getGuildId() }).get();
			DiscordGuild discordGuild(guild.data);
			EmbedData msgEmbed;
			msgEmbed.setAuthor(args->eventData.getUserName(), args->eventData.getAvatarURL());
			msgEmbed.setColor(discordGuild.data.borderColor);
			msgEmbed.setDescription("Nicely done, you've registered some commands!");
			msgEmbed.setTimeStamp(getTimeAndDate());
			msgEmbed.setTitle("__**Register Slash Commands Complete:**__");
			if (args->eventData.eventType == InputEventType::REGULAR_MESSAGE) {
				ReplyMessageData responseData(args->eventData);
				responseData.embeds.push_back(msgEmbed);
				InputEventData  event01 = InputEventManager::respondToEvent(responseData);
			}
			else if (args->eventData.eventType == InputEventType::SLASH_COMMAND_INTERACTION) {
				CreateInteractionResponseData responseData(args->eventData);
				responseData.data.embeds.push_back(msgEmbed);
				InputEventData event;
				event = InputEventManager::respondToEvent(responseData);
			}
			co_return;
			
		}
	};
	RegisterSlashCommands registerSlashCommands{};
}
#endif
