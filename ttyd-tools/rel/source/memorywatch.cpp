#include "memorywatch.h"
#include "global.h"
#include "commonfunctions.h"
#include "menufunctions.h"

#include <ttyd/__mem.h>
#include <ttyd/system.h>

#include <cstdio>

namespace mod {

int32_t getEmptyWatchSlot()
{
	int32_t Size = sizeof(MemoryWatch) / sizeof(MemoryWatch[0]);
	for (int32_t i = 0; i < Size; i++)
	{
		if (MemoryWatch[i].Address == 0)
		{
			return i;
		}
	}
	return NO_SLOTS_LEFT;
}

const char *getAddressStringNoLetterP(int32_t slot)
{
	char *tempDisplayBuffer = DisplayBuffer;
	sprintf(tempDisplayBuffer,
		"0x%08lX",
		MemoryWatch[slot].Address);
	
	return tempDisplayBuffer;
}

const char *getAddressString(int32_t slot)
{
	// Check if the current address uses pointers or not
	const char *tempPointerText;
	if (MemoryWatch[slot].AddressOffsetAmount > 0)
	{
		#ifndef TTYD_JP
		tempPointerText = "P=";
		#else
		tempPointerText = PointerText;
		#endif
	}
	else
	{
		tempPointerText = "";
	}
	
	char *tempDisplayBuffer = DisplayBuffer;
	sprintf(tempDisplayBuffer,
		"%s0x%08lX",
		tempPointerText,
		MemoryWatch[slot].Address);
	
	return tempDisplayBuffer;
}

const char *getAddressStringFromOffsets(int32_t slot, uint32_t maxOffset)
{
	int32_t OffsetArray[maxOffset];
	for (uint32_t i = 0; i < maxOffset; i++)
	{
		OffsetArray[i] = MemoryWatch[slot].AddressOffset[i];
	}
	
	uint32_t Address = reinterpret_cast<uint32_t>(
		getLastPointerFromPath(reinterpret_cast<void *>(
			MemoryWatch[slot].Address), OffsetArray, maxOffset));
	
	// Make sure the address is valid
	if ((Address < 0x80000000) || (Address >= 0x81800000))
	{
		// Address is invalid
		return "???";
	}
	
	char *tempDisplayBuffer = DisplayBuffer;
	sprintf(tempDisplayBuffer,
		"0x%08lX",
		Address);
	
	return tempDisplayBuffer;
}

const char *getValueString(int32_t slot)
{
	uint32_t Address = reinterpret_cast<uint32_t>(
		getLastPointerFromPath(reinterpret_cast<void *>(MemoryWatch[slot].Address), 
			MemoryWatch[slot].AddressOffset, MemoryWatch[slot].AddressOffsetAmount));
	
	// Make sure the address is valid
	if ((Address < 0x80000000) || (Address >= 0x81800000))
	{
		// Address is invalid
		return "???";
	}
	
	bool ShowAsHex = MemoryWatch[slot].ShowAsHex;
	char *tempDisplayBuffer = DisplayBuffer;
	
	switch (MemoryWatch[slot].Type)
	{
		case string:
		{
			return reinterpret_cast<const char *>(Address);
		}
		case time:
		{
			uint32_t TimeBase = *reinterpret_cast<uint32_t *>(ConsoleBusSpeedAddress) / 4;
			
			uint32_t FPS = *reinterpret_cast<uint32_t *>(
				*reinterpret_cast<uint32_t *>(GlobalWorkPointer) + 0x4);
			
			uint64_t CurrentTime = *reinterpret_cast<uint64_t *>(Address) / (TimeBase / FPS);
			
			uint32_t hour = CurrentTime / 3600 / FPS;
			uint32_t minute = (CurrentTime / 60 / FPS) % 60;
			uint32_t second = (CurrentTime / FPS) % 60;
			uint32_t frame = CurrentTime % FPS;
			
			sprintf(tempDisplayBuffer,
				"%02ld:%02ld:%02ld.%02ld",
				hour,
				minute,
				second,
				frame);
			return tempDisplayBuffer;
		}
		case s8:
		{
			int8_t Value = *reinterpret_cast<int8_t *>(Address);
			if (ShowAsHex)
			{
				sprintf(tempDisplayBuffer,
					"0x%02X",
					Value);
			}
			else
			{
				sprintf(tempDisplayBuffer,
					"%d",
					Value);
			}
			return tempDisplayBuffer;
		}
		case s16:
		{
			int16_t Value = *reinterpret_cast<int16_t *>(Address);
			if (ShowAsHex)
			{
				sprintf(tempDisplayBuffer,
					"0x%04X",
					Value);
			}
			else
			{
				sprintf(tempDisplayBuffer,
					"%d",
					Value);
			}
			return tempDisplayBuffer;
		}
		case s32:
		{
			int32_t Value = *reinterpret_cast<int32_t *>(Address);
			if (ShowAsHex)
			{
				sprintf(tempDisplayBuffer,
					"0x%08lX",
					Value);
			}
			else
			{
				sprintf(tempDisplayBuffer,
					"%ld",
					Value);
			}
			return tempDisplayBuffer;
		}
		case s64:
		{
			int64_t Value = *reinterpret_cast<int64_t *>(Address);
			if (ShowAsHex)
			{
				sprintf(tempDisplayBuffer,
					"0x%016llX",
					Value);
			}
			else
			{
				sprintf(tempDisplayBuffer,
					"%lld",
					Value);
			}
			return tempDisplayBuffer;
		}
		case u8:
		{
			uint8_t Value = *reinterpret_cast<uint8_t *>(Address);
			if (ShowAsHex)
			{
				sprintf(tempDisplayBuffer,
					"0x%02X",
					Value);
			}
			else
			{
				sprintf(tempDisplayBuffer,
					"%d",
					Value);
			}
			return tempDisplayBuffer;
		}
		case u16:
		{
			uint16_t Value = *reinterpret_cast<uint16_t *>(Address);
			if (ShowAsHex)
			{
				sprintf(tempDisplayBuffer,
					"0x%04X",
					Value);
			}
			else
			{
				sprintf(tempDisplayBuffer,
					"%d",
					Value);
			}
			return tempDisplayBuffer;
		}
		case u32:
		{
			uint32_t Value = *reinterpret_cast<uint32_t *>(Address);
			if (ShowAsHex)
			{
				sprintf(tempDisplayBuffer,
					"0x%08lX",
					Value);
			}
			else
			{
				sprintf(tempDisplayBuffer,
					"%ld",
					Value);
			}
			return tempDisplayBuffer;
		}
		case u64:
		{
			uint64_t Value = *reinterpret_cast<uint64_t *>(Address);
			if (ShowAsHex)
			{
				sprintf(tempDisplayBuffer,
					"0x%016llX",
					Value);
			}
			else
			{
				sprintf(tempDisplayBuffer,
					"%lld",
					Value);
			}
			return tempDisplayBuffer;
		}
		case f32:
		{
			if (ShowAsHex)
			{
				sprintf(tempDisplayBuffer,
					"0x%08lX",
					*reinterpret_cast<uint32_t *>(Address));
			}
			else
			{
				sprintf(tempDisplayBuffer,
					"%.2f",
					*reinterpret_cast<float *>(Address));
			}
			return tempDisplayBuffer;
		}
		case f64:
		{
			if (ShowAsHex)
			{
				sprintf(tempDisplayBuffer,
					"0x%016llX",
					*reinterpret_cast<uint64_t *>(Address));
			}
			else
			{
				sprintf(tempDisplayBuffer,
					"%.2f",
					*reinterpret_cast<double *>(Address));
			}
			return tempDisplayBuffer;
		}
		default:
		{
			return "";
		}
	}
}

void addMemoryWatch(int32_t slot)
{
	MemoryWatch[slot].Address 		= 0x80000000;
	MemoryWatch[slot].Type 			= string;
	MemoryWatch[slot].ShowAsHex 	= false;
	MemoryWatch[slot].Display 		= false;
	MemoryWatch[slot].PosX 			= -44;
	MemoryWatch[slot].PosY 			= -32;
}

void deleteWatch(int32_t slot)
{
	int32_t TotalSlots = sizeof(MemoryWatch) / sizeof(MemoryWatch[0]);
	int32_t TotalMenuOptions = 0;
	
	for (int32_t i = 0; i < TotalSlots; i++)
	{
		if (MemoryWatch[i].Address == 0)
		{
			break;
		}
		TotalMenuOptions++;
	}
	
	// Adjust for 0 index
	TotalMenuOptions--;
	
	if (slot < TotalMenuOptions)
	{
		uint32_t Size = sizeof(MemoryWatch[0]) * (TotalMenuOptions - slot);
		ttyd::__mem::memcpy(&MemoryWatch[slot], &MemoryWatch[slot + 1], Size);
	}
	
	clearMemory(&MemoryWatch[TotalMenuOptions], sizeof(MemoryWatch[0]));
	TotalMenuOptions--;
	
	// Check if there are any watches left
	if (TotalMenuOptions < 0)
	{
		closeSecondaryMenu();
		return;
	}
	
	// Make sure the currently selected option is valid
	uint32_t tempCurrentMenuOption = CurrentMenuOption;
	if (tempCurrentMenuOption > static_cast<uint32_t>(TotalMenuOptions))
	{
		CurrentMenuOption = tempCurrentMenuOption - 1;
	}
	
	// Make sure the current page is valid
	uint32_t tempCurrentPage = CurrentPage;
	if ((tempCurrentPage * 10) > static_cast<uint32_t>(TotalMenuOptions))
	{
		CurrentPage = tempCurrentPage - 1;
	}
}

uint32_t adjustWatchValueControls(int32_t slot)
{
	uint32_t tempCurrentMenu 		= CurrentMenu;
	uint32_t tempCurrentMenuOption 	= CurrentMenuOption;
	
	// Get the amount of numbers to draw
	bool NoNumbersToDisplay = false;
	uint32_t HighestDigit = 0;
	uint32_t Button = 0;
	
	switch (tempCurrentMenu)
	{
		case MEMORY_CHANGE_ADDRESS:
		{
			HighestDigit = 8;
			break;
		}
		default:
		{
			NoNumbersToDisplay = true;
			Button = B;
			break;
		}
	}
	
	// Make sure there is at least 1 number to display
	if (!NoNumbersToDisplay)
	{
		// Check to see if D-Pad Up or D-Pad Down are being held
		if (checkButtonComboEveryFrame(PAD_DPAD_UP))
		{
			Button = DPADUP;
		}
		else if (checkButtonComboEveryFrame(PAD_DPAD_DOWN))
		{
			Button = DPADDOWN;
		}
		
		if (Button != 0)
		{
			// Check to see if the value should begin to auto-increment
			if (MemoryWatchAdjustableValueMenu.WaitFramesToBeginIncrement >= 
				ttyd::system::sysMsec2Frame(500))
			{
				// Check to see if the number should increment or not
				if (MemoryWatchAdjustableValueMenu.WaitFramesToPerformIncrement >= 1)
				{
					// Auto-increment the value
					int32_t IncrementAmount;
					if (Button == DPADUP)
					{
						IncrementAmount = 1;
					}
					else // (Button == DPADDOWN)
					{
						IncrementAmount = -1;
					}
					
					adjustWatchTempValueAndBounds(slot, HighestDigit, IncrementAmount);
					MemoryWatchAdjustableValueMenu.WaitFramesToPerformIncrement = 0;
					return Button;
				}
				else
				{
					MemoryWatchAdjustableValueMenu.WaitFramesToPerformIncrement++;
				}
			}
			else
			{
				MemoryWatchAdjustableValueMenu.WaitFramesToBeginIncrement++;
			}
		}
		else
		{
			// Reset the counters
			MemoryWatchAdjustableValueMenu.WaitFramesToBeginIncrement = 0;
			MemoryWatchAdjustableValueMenu.WaitFramesToPerformIncrement = 0;
		}
		
		Button = checkButtonSingleFrame();
	}
	
	switch (Button)
	{
		case DPADLEFT:
		{
			uint32_t tempSecondaryMenuOption = SecondaryMenuOption;
			if (SecondaryMenuOption == 0)
			{
				// Loop to the last option
				SecondaryMenuOption = HighestDigit - 1;
			}
			else
			{
				// Move left once
				SecondaryMenuOption = tempSecondaryMenuOption - 1;
			}
			
			FrameCounter = 1;
			return Button;
		}
		case DPADRIGHT:
		{
			uint32_t tempSecondaryMenuOption = SecondaryMenuOption;
			if (SecondaryMenuOption == (HighestDigit - 1))
			{
				// Loop to the first option
				SecondaryMenuOption = 0;
			}
			else
			{
				// Move right once
				SecondaryMenuOption = tempSecondaryMenuOption + 1;
			}
			
			FrameCounter = 1;
			return Button;
		}
		case DPADDOWN:
		{
			// Decrement the current value for the current slot
			adjustWatchTempValueAndBounds(slot, HighestDigit, -1);
			
			FrameCounter = 1;
			return Button;
		}
		case DPADUP:
		{
			// Increment the current value for the current slot
			adjustWatchTempValueAndBounds(slot, HighestDigit, 1);
			
			FrameCounter = 1;
			return Button;
		}
		case A:
		{
			switch (tempCurrentMenu)
			{
				case MEMORY_CHANGE_ADDRESS:
				{
					switch (tempCurrentMenuOption)
					{
						case 0:
						{
							// Modifying the address
							MemoryWatch[slot].Address = MemoryWatchSecondaryValue;
							MenuSelectionStates = 0;
							
							FrameCounter = 1;
							return Button;
						}
						default:
						{
							// Modifying the pointer offsets
							MemoryWatch[slot].AddressOffset[tempCurrentMenuOption - 1] = 
								static_cast<int32_t>(MemoryWatchSecondaryValue);
							MenuSelectionStates = 0;
							
							FrameCounter = 1;
							return Button;
						}
					}
					break;
				}
				default:
				{
					return 0;
				}
			}
			break;
		}
		case B:
		{
			switch (tempCurrentMenu)
			{
				case MEMORY_CHANGE_ADDRESS:
				{
					MenuSelectionStates = 0;
					
					FrameCounter = 1;
					
					if (!NoNumbersToDisplay)
					{
						return Button;
					}
					else
					{
						return NO_NUMBERS_TO_DISPLAY;
					}
				}
				default:
				{
					return 0;
				}
			}
			break;
		}
		default:
		{
			return 0;
		}
	}
}

void adjustWatchTempValueAndBounds(int32_t slot, uint32_t highestDigit, int32_t valueChangedBy)
{
	for (uint32_t i = 0; i < (highestDigit - SecondaryMenuOption - 1); i++)
	{
		valueChangedBy *= 0x10;
	}
	
	uint32_t tempMemoryWatchSecondaryValue = MemoryWatchSecondaryValue + valueChangedBy;
	MemoryWatchSecondaryValue = tempMemoryWatchSecondaryValue;
	
	switch (CurrentMenu)
	{
		case MEMORY_CHANGE_ADDRESS:
		{
			uint32_t UpperBoundUnsigned;
			uint32_t LowerBoundUnsigned;
			int32_t UpperBoundSigned;
			int32_t LowerBoundSigned;
			
			switch (CurrentMenuOption)
			{
				case 0:
				{
					// Modifying the address
					UpperBoundUnsigned = 0x817FFFFF;
					LowerBoundUnsigned = 0x80000000;
					
					if (tempMemoryWatchSecondaryValue > UpperBoundUnsigned)
					{
						// Loop to the beginning
						MemoryWatchSecondaryValue = LowerBoundUnsigned;
					}
					else if (tempMemoryWatchSecondaryValue < LowerBoundUnsigned)
					{
						// Loop to the end
						MemoryWatchSecondaryValue = UpperBoundUnsigned;
					}
					break;
				}
				default:
				{
					// Modifying the pointer offsets
					UpperBoundSigned = 0x7FFFFFFF;
					LowerBoundSigned = -0x80000000;
					
					int32_t tempMemoryWatchSecondaryValueSigned = static_cast<int32_t>(tempMemoryWatchSecondaryValue);
					if (tempMemoryWatchSecondaryValueSigned > UpperBoundSigned)
					{
						// Loop to the beginning
						MemoryWatchSecondaryValue = static_cast<uint32_t>(LowerBoundSigned);
					}
					else if (tempMemoryWatchSecondaryValueSigned < LowerBoundSigned)
					{
						// Loop to the end
						MemoryWatchSecondaryValue = static_cast<uint32_t>(UpperBoundSigned);
					}
					break;
				}
			}
			break;
		}
		default:
		{
			return;
		}
	}
}

}