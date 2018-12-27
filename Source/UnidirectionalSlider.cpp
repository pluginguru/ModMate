#include "UnidirectionalSlider.h"

void UnidirectionalSlider::paint (Graphics& g)
{
    auto area = getLocalBounds();

    g.setColour(Colours::black);
    g.fillRect(area);

    if (value > 0.0f)
    {
        g.setColour(colour);
        if (area.getWidth() > area.getHeight())
        {
            g.fillRect(Rectangle<int>(area.getX(),
                area.getY(),
                int(value * area.getWidth()),
                area.getHeight()));
        }
        else
        {
            g.fillRect(Rectangle<int>(area.getX(),
                area.getY() + int((1.0f - value) * area.getHeight()),
                area.getWidth(),
                int(value * area.getHeight())));
        }
    }

    g.setColour(Colours::white);
    g.drawRect(area);
}

void UnidirectionalSlider::mouseDrag(const MouseEvent& evt)
{
    float fv = (getWidth() > getHeight())
                ? evt.getPosition().getX() / float(getWidth())
                : 1.0f - evt.getPosition().getY() / float(getHeight());
    if (fv < 0.0f) fv = 0.0f;
    if (fv > 1.0f) fv = 1.0f;
    if (value != fv)
    {
        value = fv;
        repaint();
        sendChangeMessage();
    }
}

void UnidirectionalSlider::mouseUp(const MouseEvent&)
{
    value = 0.0f;
    repaint();
    sendChangeMessage();
}


void UnidirectionalSlider::setValue(float v)
{
    value = v;
    if (value < 0.0f) value = 0.0f;
    if (value > 1.0f) value = 1.0f;
	repaint();
}
