#include "MusicFile.h"
#include "../../Utility/MIX_Exception.h"
#include <assert.h>

MusicFile::MusicFile(const std::string & filename, float vol)
{
	m_pMusic = Mix_LoadWAV(filename.c_str());
	if (!m_pMusic)
		throw MIX_Exception("music file error - " + filename);

	setVolume(vol);
}

MusicFile::~MusicFile()
{
	if (m_pMusic)
	{
		stop();

		Mix_FreeChunk(m_pMusic);
		m_pMusic = nullptr;
	}
}

void MusicFile::play(bool repeat)
{
	// play on next free channel
	m_channel = Mix_PlayChannel(-1, m_pMusic, repeat? -1 : 0);
}

void MusicFile::stop()
{
	if (m_channel != -1)
	{
		Mix_HaltChannel(m_channel);
		m_channel = -1;
	}
}

bool MusicFile::isPlaying() const
{
	if (m_channel == -1)
		return false;
	
	int playing = Mix_Playing(m_channel);
	int paused = Mix_Paused(m_channel);

	if (!playing || paused)
		return false;

	return true;
}

void MusicFile::setVolume(float vol)
{
	m_curVol = int(vol * float(MIX_MAX_VOLUME));
	setVolumeNoSave(m_curVol);
}

void MusicFile::setMute(bool mute)
{
	m_isMuted = mute;
	setVolumeNoSave(m_isMuted ? 0 : m_curVol);
}

int MusicFile::getChannel() const
{
	assert(m_channel != -1);
	return m_channel;
}

void MusicFile::setVolumeNoSave(int vol) const
{
	Mix_VolumeChunk(m_pMusic, vol);

	if (m_channel != -1)
	{
		Mix_Volume(m_channel, vol);
	}
}
