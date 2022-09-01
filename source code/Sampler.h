#pragma once
#include <SFML/Audio.hpp>
#include <vector>

class SoundVisualisalizer :public sf::Sound
{
public:
    SoundVisualisalizer();
    SoundVisualisalizer(const sf::SoundBuffer& buffer);
    void calculateElapsedSamples();
    std::vector<sf::Int16>& getSampleArray();
private:
    sf::Uint64 m_lastSamplePosition;
    sf::Uint64 m_currentSamplePosition;
    std::vector<sf::Int16> m_CurrentSampleArray;

};

SoundVisualisalizer::SoundVisualisalizer()
    :m_lastSamplePosition(0),
    m_currentSamplePosition(0)
{}

SoundVisualisalizer::SoundVisualisalizer(const sf::SoundBuffer& buffer)
    : m_lastSamplePosition(0),
    m_currentSamplePosition(0)
{
    this->setBuffer(buffer);
}

std::vector<sf::Int16>& SoundVisualisalizer::getSampleArray()
{
    return m_CurrentSampleArray;
}

// Creates an array of music samples for future use, starting at part of the song that is currently played.
void SoundVisualisalizer::calculateElapsedSamples()
{
    if (this->getStatus() == sf::SoundSource::Playing)
    {
        sf::Int64 s_time = this->getPlayingOffset().asMilliseconds();
        sf::Int64 s_rate = this->getBuffer()->getSampleRate();
        sf::Int64 s_chanel = this->getBuffer()->getChannelCount();
        sf::Uint64 s_count = this->getBuffer()->getSampleCount();
        const sf::Int16* s_begin = this->getBuffer()->getSamples();

        m_currentSamplePosition = s_time * s_rate * s_chanel / 1000;
        if (m_currentSamplePosition <= s_count)
            m_CurrentSampleArray.assign(s_begin + m_lastSamplePosition, s_begin + m_currentSamplePosition);
        else m_currentSamplePosition = s_count;
        m_lastSamplePosition = m_currentSamplePosition;
    }
}