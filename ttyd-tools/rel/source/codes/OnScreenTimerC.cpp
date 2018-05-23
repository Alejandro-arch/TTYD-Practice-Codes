#include "mod.h"

#include <ttyd/system.h>
#include <ttyd/fontmgr.h>

#include <cstdio>

extern uint16_t EnableOrResetTimerCombo;
extern uint16_t DisableTimerCombo;
extern uint16_t StartOrResumeTimerCombo;
extern uint16_t PauseTimerCombo;

namespace mod {

void Mod::onScreenTimer()
{
	uint32_t ButtonInput = ttyd::system::keyGetButton(0);
	uint32_t timerCount = mOnScreenTimer.getValue();
	
	if ((ButtonInput & EnableOrResetTimerCombo) == (EnableOrResetTimerCombo))
	{
		// Enable/Reset Timer
		mOnScreenTimerEnabled = true;
		mOnScreenTimer.stop();
		mOnScreenTimer.setValue(0);
	}
	
	if (mOnScreenTimerEnabled)
	{
		if ((ButtonInput & DisableTimerCombo) == (DisableTimerCombo))
		{
			// Disable Timer
			mOnScreenTimerEnabled = false;
			mOnScreenTimer.stop();
		}
		else if ((ButtonInput & StartOrResumeTimerCombo) == (StartOrResumeTimerCombo))
		{
			// Start/Resume Timer
			mOnScreenTimer.start();
		}
		else if ((ButtonInput & PauseTimerCombo) == (PauseTimerCombo))
		{
			// Pause Timer
			mOnScreenTimer.stop();
		}
		
		uint32_t modframe = timerCount % 60;
		uint32_t second = (timerCount / 60) % 60;
		uint32_t minute = (timerCount / (60 * 60)) % 60;
		uint32_t hour = timerCount / (60 * 60 * 60);
		
		sprintf(mDisplayBuffer,
			"%.2d:%.2d:%.2d.%.2d",
			hour,
			minute,
			second,
			modframe);
		
		ttyd::fontmgr::FontDrawStart();
		uint32_t color = 0xFFFFFFFF;
		ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
		ttyd::fontmgr::FontDrawEdge();
		ttyd::fontmgr::FontDrawMessage(100, -153, mDisplayBuffer);
		
		mOnScreenTimer.tick();
	}
	
}

}