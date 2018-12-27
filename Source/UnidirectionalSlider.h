#pragma once

#include "JuceHeader.h"

class UnidirectionalSlider	: public Component, public ChangeBroadcaster
{
public:
    UnidirectionalSlider(Colour c) : colour(c), value(0.0f) {}
    virtual ~UnidirectionalSlider() = default;

	// Component
	void paint(Graphics&) override;
    void mouseDrag(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;

	// UnidirectionalSlider
	void setValue(float v);
    float getValue() { return value; }

private:
    Colour colour;
    float value;
};
