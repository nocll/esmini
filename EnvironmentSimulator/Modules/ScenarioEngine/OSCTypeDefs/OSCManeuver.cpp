/*
 * esmini - Environment Simulator Minimalistic
 * https://github.com/esmini/esmini
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) partners of Simulation Scenarios
 * https://sites.google.com/view/simulationscenarios
 */

#include "OSCManeuver.hpp"

using namespace scenarioengine;

void Event::Start(double simTime, double dt)
{
	for (size_t i = 0; i < action_.size(); i++)
	{
		// Restart actions
		action_[i]->Reset();
		action_[i]->Start(simTime, dt);

		// When using a TeleportAction for the Ghost-vehicle, we need to set back the starting simTime for other Actions in the same Event.
		// This is an easy solution. A nicer one could be to access ScenarioEngines getSimulationTime() when calling action Start.
		OSCAction* action = action_[i];
		OSCPrivateAction* pa = (OSCPrivateAction*)action;
		if (pa->object_->IsGhost() && pa->type_ == OSCPrivateAction::ActionType::TELEPORT)
		{
			simTime -= pa->object_->GetHeadstartTime();
		}

	}
	StoryBoardElement::Start(simTime, dt);
}

void Event::End()
{
	for (size_t i = 0; i < action_.size(); i++)
	{
		if (action_[i]->IsActive())
		{
			action_[i]->End();
		}
	}
	StoryBoardElement::End();
}

void Event::Stop()
{
	for (size_t i = 0; i < action_.size(); i++)
	{
		action_[i]->Stop();
	}
	StoryBoardElement::Stop();
}

bool scenarioengine::OSCManeuver::IsAnyEventActive()
{
	for (size_t i = 0; i < event_.size(); i++)
	{
		if (event_[i]->IsActive())
		{
			return true;
		}
	}
	return false;
}
