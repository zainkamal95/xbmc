/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "DataCacheCore.h"

#include "DVDStreamInfo.h"
#include "ServiceBroker.h"
#include "cores/EdlEdit.h"
#include "cores/AudioEngine/Utils/AEStreamInfo.h"
#include "utils/AgedMap.h"
#include "utils/BitstreamConverter.h"

#include <mutex>
#include <utility>

CDataCacheCore::CDataCacheCore() :
  m_playerVideoInfo {},
  m_playerAudioInfo {},
  m_contentInfo {},
  m_renderInfo {},
  m_stateInfo {}
{
}

CDataCacheCore::~CDataCacheCore() = default;

CDataCacheCore& CDataCacheCore::GetInstance()
{
  return CServiceBroker::GetDataCacheCore();
}

void CDataCacheCore::Reset()
{
  {
    std::unique_lock<CCriticalSection> lock(m_stateSection);
    m_stateInfo = {};
    m_playerStateChanged = false;
  }
  {
    std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);
    m_playerVideoInfo = {};
  }
  {
    std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);
    m_playerAudioInfo = {};
  }
  m_hasAVInfoChanges = false;
  {
    std::unique_lock<CCriticalSection> lock(m_renderSection);
    m_renderInfo = {};
  }
  {
    std::unique_lock<CCriticalSection> lock(m_contentSection);
    m_contentInfo.Reset();
  }
  m_timeInfo = {};
}

bool CDataCacheCore::HasAVInfoChanges()
{
  bool ret = m_hasAVInfoChanges;
  m_hasAVInfoChanges = false;
  return ret;
}

void CDataCacheCore::SignalVideoInfoChange()
{
  m_hasAVInfoChanges = true;
}

void CDataCacheCore::SignalAudioInfoChange()
{
  m_hasAVInfoChanges = true;
}

void CDataCacheCore::SignalSubtitleInfoChange()
{
  m_hasAVInfoChanges = true;
}

void CDataCacheCore::SetAVChange(bool value)
{
  m_AVChange = value;
}

bool CDataCacheCore::GetAVChange()
{
  return m_AVChange;
}

void CDataCacheCore::SetVideoDecoderName(std::string name, bool isHw)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.decoderName = std::move(name);
  m_playerVideoInfo.isHwDecoder = isHw;
}

std::string CDataCacheCore::GetVideoDecoderName()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.decoderName;
}

bool CDataCacheCore::IsVideoHwDecoder()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.isHwDecoder;
}

void CDataCacheCore::SetVideoDeintMethod(std::string method)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.deintMethod = std::move(method);
}

std::string CDataCacheCore::GetVideoDeintMethod()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.deintMethod;
}

void CDataCacheCore::SetVideoPixelFormat(std::string pixFormat)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.pixFormat = std::move(pixFormat);
}

std::string CDataCacheCore::GetVideoPixelFormat()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.pixFormat;
}

void CDataCacheCore::SetVideoStereoMode(std::string mode)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.stereoMode = std::move(mode);
}

std::string CDataCacheCore::GetVideoStereoMode()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.stereoMode;
}

void CDataCacheCore::SetVideoDimensions(int width, int height)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.width = width;
  m_playerVideoInfo.height = height;
}

int CDataCacheCore::GetVideoWidth()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.width;
}

int CDataCacheCore::GetVideoHeight()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.height;
}

void CDataCacheCore::SetVideoBitDepth(int bitDepth)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.bitDepth = bitDepth;
}

int CDataCacheCore::GetVideoBitDepth()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.bitDepth;
}

void CDataCacheCore::SetVideoHdrType(StreamHdrType hdrType)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.hdrType = hdrType;
}

StreamHdrType CDataCacheCore::GetVideoHdrType()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.hdrType;
}

void CDataCacheCore::SetVideoSourceHdrType(StreamHdrType hdrType)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.sourceHdrType = hdrType;
}

StreamHdrType CDataCacheCore::GetVideoSourceHdrType()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.sourceHdrType;
}

void CDataCacheCore::SetVideoSourceAdditionalHdrType(StreamHdrType hdrType)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.sourceAdditionalHdrType = hdrType;
}

StreamHdrType CDataCacheCore::GetVideoSourceAdditionalHdrType()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.sourceAdditionalHdrType;
}

void CDataCacheCore::SetVideoColorSpace(AVColorSpace colorSpace)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.colorSpace = colorSpace;
}

AVColorSpace CDataCacheCore::GetVideoColorSpace()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.colorSpace;
}

void CDataCacheCore::SetVideoColorRange(AVColorRange colorRange)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.colorRange = colorRange;
}

AVColorRange CDataCacheCore::GetVideoColorRange()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.colorRange;
}

void CDataCacheCore::SetVideoColorPrimaries(AVColorPrimaries colorPrimaries)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.colorPrimaries = colorPrimaries;
}

AVColorPrimaries CDataCacheCore::GetVideoColorPrimaries()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.colorPrimaries;
}

void CDataCacheCore::SetVideoColorTransferCharacteristic(AVColorTransferCharacteristic colorTransferCharacteristic)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.colorTransferCharacteristic = colorTransferCharacteristic;
}

AVColorTransferCharacteristic CDataCacheCore::GetVideoColorTransferCharacteristic()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.colorTransferCharacteristic;
}

void CDataCacheCore::SetVideoDoViFrameMetadata(DOVIFrameMetadata value)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.doviFrameMetadataMap.insert(value.pts, value);
}

DOVIFrameMetadata CDataCacheCore::GetVideoDoViFrameMetadata()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  auto doviFrameMetadata = m_playerVideoInfo.doviFrameMetadataMap.findOrLatest(GetRenderPts());
  if (doviFrameMetadata != m_playerVideoInfo.doviFrameMetadataMap.end())
    return doviFrameMetadata->second;
  return {};
}

void CDataCacheCore::SetVideoDoViStreamMetadata(DOVIStreamMetadata value)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.doviStreamMetadata = value;
}

DOVIStreamMetadata CDataCacheCore::GetVideoDoViStreamMetadata()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.doviStreamMetadata;
}

void CDataCacheCore::SetVideoDoViStreamInfo(DOVIStreamInfo value)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.doviStreamInfo = value;
}

DOVIStreamInfo CDataCacheCore::GetVideoDoViStreamInfo()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.doviStreamInfo;
}

void CDataCacheCore::SetVideoSourceDoViStreamInfo(DOVIStreamInfo value)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.sourceDoViStreamInfo = value;
}

DOVIStreamInfo CDataCacheCore::GetVideoSourceDoViStreamInfo()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.sourceDoViStreamInfo;
}

void CDataCacheCore::SetVideoDoViCodecFourCC(std::string codecFourCC)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.doviCodecFourCC = codecFourCC;
}

std::string CDataCacheCore::GetVideoDoViCodecFourCC()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.doviCodecFourCC;
}

void CDataCacheCore::SetVideoHDRStaticMetadataInfo(HDRStaticMetadataInfo value)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.hdrStaticMetadataInfo = value;
}

HDRStaticMetadataInfo CDataCacheCore::GetVideoHDRStaticMetadataInfo()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.hdrStaticMetadataInfo;
}

void CDataCacheCore::SetVideoLiveBitRate(double bitRate)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.liveBitRate = bitRate;
}

double CDataCacheCore::GetVideoLiveBitRate()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.liveBitRate;
}

void CDataCacheCore::SetVideoQueueLevel(int level)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.queueLevel = level;
}

int CDataCacheCore::GetVideoQueueLevel()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.queueLevel;
}

void CDataCacheCore::SetVideoQueueDataLevel(int level)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.queueDataLevel = level;
}

int CDataCacheCore::GetVideoQueueDataLevel()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.queueDataLevel;
}

void CDataCacheCore::SetVideoFps(float fps)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.fps = fps;
}

float CDataCacheCore::GetVideoFps()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.fps;
}

void CDataCacheCore::SetVideoDAR(float dar)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  m_playerVideoInfo.dar = dar;
}

float CDataCacheCore::GetVideoDAR()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);

  return m_playerVideoInfo.dar;
}

void CDataCacheCore::SetVideoInterlaced(bool isInterlaced)
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);
  m_playerVideoInfo.m_isInterlaced = isInterlaced;
}

bool CDataCacheCore::IsVideoInterlaced()
{
  std::unique_lock<CCriticalSection> lock(m_videoPlayerSection);
  return m_playerVideoInfo.m_isInterlaced;
}

// player audio info
void CDataCacheCore::SetAudioDecoderName(std::string name)
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  m_playerAudioInfo.decoderName = std::move(name);
}

std::string CDataCacheCore::GetAudioDecoderName()
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  return m_playerAudioInfo.decoderName;
}

void CDataCacheCore::SetAudioChannels(std::string channels)
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  m_playerAudioInfo.channels = std::move(channels);
}

std::string CDataCacheCore::GetAudioChannels()
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  return m_playerAudioInfo.channels;
}

void CDataCacheCore::SetAudioSampleRate(int sampleRate)
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  m_playerAudioInfo.sampleRate = sampleRate;
}

int CDataCacheCore::GetAudioSampleRate()
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  return m_playerAudioInfo.sampleRate;
}

void CDataCacheCore::SetAudioBitsPerSample(int bitsPerSample)
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  m_playerAudioInfo.bitsPerSample = bitsPerSample;
}

int CDataCacheCore::GetAudioBitsPerSample()
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  return m_playerAudioInfo.bitsPerSample;
}

void CDataCacheCore::SetAudioIsDolbyAtmos(bool isDolbyAtmos)
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  m_playerAudioInfo.isDolbyAtmos = isDolbyAtmos;
}

bool CDataCacheCore::GetAudioIsDolbyAtmos()
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  return m_playerAudioInfo.isDolbyAtmos;
}

void CDataCacheCore::SetAudioDtsXType(DtsXType dtsXType)
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  m_playerAudioInfo.dtsXType = dtsXType;
}

DtsXType CDataCacheCore::GetAudioDtsXType()
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  return m_playerAudioInfo.dtsXType;
}

void CDataCacheCore::SetAudioLiveBitRate(double bitRate)
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  m_playerAudioInfo.liveBitRate = bitRate;
}

double CDataCacheCore::GetAudioLiveBitRate()
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  return m_playerAudioInfo.liveBitRate;
}

void CDataCacheCore::SetAudioQueueLevel(int level)
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  m_playerAudioInfo.queueLevel = level;
}

int CDataCacheCore::GetAudioQueueLevel()
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  return m_playerAudioInfo.queueLevel;
}

void CDataCacheCore::SetAudioQueueDataLevel(int level)
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  m_playerAudioInfo.queueDataLevel = level;
}

int CDataCacheCore::GetAudioQueueDataLevel()
{
  std::unique_lock<CCriticalSection> lock(m_audioPlayerSection);

  return m_playerAudioInfo.queueDataLevel;
}

void CDataCacheCore::SetEditList(const std::vector<EDL::Edit>& editList)
{
  std::unique_lock<CCriticalSection> lock(m_contentSection);
  m_contentInfo.SetEditList(editList);
}

const std::vector<EDL::Edit>& CDataCacheCore::GetEditList() const
{
  std::unique_lock<CCriticalSection> lock(m_contentSection);
  return m_contentInfo.GetEditList();
}

void CDataCacheCore::SetCuts(const std::vector<int64_t>& cuts)
{
  std::unique_lock<CCriticalSection> lock(m_contentSection);
  m_contentInfo.SetCuts(cuts);
}

const std::vector<int64_t>& CDataCacheCore::GetCuts() const
{
  std::unique_lock<CCriticalSection> lock(m_contentSection);
  return m_contentInfo.GetCuts();
}

void CDataCacheCore::SetSceneMarkers(const std::vector<int64_t>& sceneMarkers)
{
  std::unique_lock<CCriticalSection> lock(m_contentSection);
  m_contentInfo.SetSceneMarkers(sceneMarkers);
}

const std::vector<int64_t>& CDataCacheCore::GetSceneMarkers() const
{
  std::unique_lock<CCriticalSection> lock(m_contentSection);
  return m_contentInfo.GetSceneMarkers();
}

void CDataCacheCore::SetChapters(const std::vector<std::pair<std::string, int64_t>>& chapters)
{
  std::unique_lock<CCriticalSection> lock(m_contentSection);
  m_contentInfo.SetChapters(chapters);
}

const std::vector<std::pair<std::string, int64_t>>& CDataCacheCore::GetChapters() const
{
  std::unique_lock<CCriticalSection> lock(m_contentSection);
  return m_contentInfo.GetChapters();
}

void CDataCacheCore::SetRenderClockSync(bool enable)
{
  std::unique_lock<CCriticalSection> lock(m_renderSection);

  m_renderInfo.m_isClockSync = enable;
}

bool CDataCacheCore::IsRenderClockSync()
{
  std::unique_lock<CCriticalSection> lock(m_renderSection);

  return m_renderInfo.m_isClockSync;
}

void CDataCacheCore::SetRenderPts(double pts)
{
  std::unique_lock<CCriticalSection> lock(m_renderSection);

  m_renderInfo.pts = pts;
}

double CDataCacheCore::GetRenderPts()
{
  std::unique_lock<CCriticalSection> lock(m_renderSection);

  return m_renderInfo.pts;
}

// player states
void CDataCacheCore::SeekFinished(int64_t offset)
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);
  m_stateInfo.m_lastSeekTime = std::chrono::system_clock::now();
  m_stateInfo.m_lastSeekOffset = offset;
}

int64_t CDataCacheCore::GetSeekOffSet() const
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);
  return m_stateInfo.m_lastSeekOffset;
}

bool CDataCacheCore::HasPerformedSeek(int64_t lastSecondInterval) const
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);
  if (m_stateInfo.m_lastSeekTime == std::chrono::time_point<std::chrono::system_clock>{})
  {
    return false;
  }
  return (std::chrono::system_clock::now() - m_stateInfo.m_lastSeekTime) <
         std::chrono::duration_cast<std::chrono::seconds>(
             std::chrono::duration<int64_t>(lastSecondInterval));
}

void CDataCacheCore::SetStateSeeking(bool active)
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  m_stateInfo.m_stateSeeking = active;
  m_playerStateChanged = true;
}

bool CDataCacheCore::IsSeeking()
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  return m_stateInfo.m_stateSeeking;
}

void CDataCacheCore::SetSpeed(float tempo, float speed)
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  m_stateInfo.m_tempo = tempo;
  m_stateInfo.m_speed = speed;
}

float CDataCacheCore::GetSpeed()
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  return m_stateInfo.m_speed;
}

float CDataCacheCore::GetTempo()
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  return m_stateInfo.m_tempo;
}

void CDataCacheCore::SetFrameAdvance(bool fa)
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  m_stateInfo.m_frameAdvance = fa;
}

bool CDataCacheCore::IsFrameAdvance()
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  return m_stateInfo.m_frameAdvance;
}

bool CDataCacheCore::IsPlayerStateChanged()
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  bool ret(m_playerStateChanged);
  m_playerStateChanged = false;

  return ret;
}

void CDataCacheCore::SetGuiRender(bool gui)
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  m_stateInfo.m_renderGuiLayer = gui;
  m_playerStateChanged = true;
}

bool CDataCacheCore::GetGuiRender()
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  return m_stateInfo.m_renderGuiLayer;
}

void CDataCacheCore::SetVideoRender(bool video)
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  m_stateInfo.m_renderVideoLayer = video;
  m_playerStateChanged = true;
}

bool CDataCacheCore::GetVideoRender()
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  return m_stateInfo.m_renderVideoLayer;
}

void CDataCacheCore::SetPlayTimes(time_t start, int64_t current, int64_t min, int64_t max)
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);
  m_timeInfo.m_startTime = start;
  m_timeInfo.m_time = current;
  m_timeInfo.m_timeMin = min;
  m_timeInfo.m_timeMax = max;
}

void CDataCacheCore::GetPlayTimes(time_t &start, int64_t &current, int64_t &min, int64_t &max)
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);
  start = m_timeInfo.m_startTime;
  current = m_timeInfo.m_time;
  min = m_timeInfo.m_timeMin;
  max = m_timeInfo.m_timeMax;
}

time_t CDataCacheCore::GetStartTime()
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);
  return m_timeInfo.m_startTime;
}

int64_t CDataCacheCore::GetPlayTime()
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);
  return m_timeInfo.m_time;
}

int64_t CDataCacheCore::GetMinTime()
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);
  return m_timeInfo.m_timeMin;
}

int64_t CDataCacheCore::GetMaxTime()
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);
  return m_timeInfo.m_timeMax;
}

float CDataCacheCore::GetPlayPercentage()
{
  std::unique_lock<CCriticalSection> lock(m_stateSection);

  // Note: To calculate accurate percentage, all time data must be consistent,
  //       which is the case for data cache core. Calculation can not be done
  //       outside of data cache core or a possibility to lock the data cache
  //       core from outside would be needed.
  int64_t iTotalTime = m_timeInfo.m_timeMax - m_timeInfo.m_timeMin;
  if (iTotalTime <= 0)
    return 0;

  return m_timeInfo.m_time * 100 / static_cast<float>(iTotalTime);
}
