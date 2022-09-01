/////////////////////////////////////////////////////////////////////////////////////
//
//  MIT License
//  
//  Fraktal Ihos - Copyright(c) 2022 Antoni Tretiakov - antonitre29@gmail.com
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this softwareand associated documentation files(the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions :
//  
//  The above copyright noticeand this permission notice shall be included in all
//  copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////

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