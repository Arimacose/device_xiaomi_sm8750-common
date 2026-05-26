/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/vibrator/BnVibrator.h>

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

namespace aidl {
namespace android {
namespace hardware {
namespace vibrator {

class Vibrator : public BnVibrator {
public:
    Vibrator();
    ~Vibrator() override;

    ndk::ScopedAStatus getCapabilities(int32_t* _aidl_return) override;
    ndk::ScopedAStatus off() override;
    ndk::ScopedAStatus on(int32_t timeoutMs,
                          const std::shared_ptr<IVibratorCallback>& callback) override;
    ndk::ScopedAStatus perform(Effect effect, EffectStrength strength,
                               const std::shared_ptr<IVibratorCallback>& callback,
                               int32_t* _aidl_return) override;
    ndk::ScopedAStatus getSupportedEffects(std::vector<Effect>* _aidl_return) override;
    ndk::ScopedAStatus setAmplitude(float amplitude) override;
    ndk::ScopedAStatus setExternalControl(bool enabled) override;
    ndk::ScopedAStatus getCompositionDelayMax(int32_t* maxDelayMs) override;
    ndk::ScopedAStatus getCompositionSizeMax(int32_t* maxSize) override;
    ndk::ScopedAStatus getSupportedPrimitives(std::vector<CompositePrimitive>* supported) override;
    ndk::ScopedAStatus getPrimitiveDuration(CompositePrimitive primitive,
                                            int32_t* durationMs) override;
    ndk::ScopedAStatus compose(const std::vector<CompositeEffect>& composite,
                               const std::shared_ptr<IVibratorCallback>& callback) override;
    ndk::ScopedAStatus getSupportedAlwaysOnEffects(std::vector<Effect>* _aidl_return) override;
    ndk::ScopedAStatus alwaysOnEnable(int32_t id, Effect effect, EffectStrength strength) override;
    ndk::ScopedAStatus alwaysOnDisable(int32_t id) override;
    ndk::ScopedAStatus getResonantFrequency(float* resonantFreqHz) override;
    ndk::ScopedAStatus getQFactor(float* qFactor) override;
    ndk::ScopedAStatus getFrequencyResolution(float* freqResolutionHz) override;
    ndk::ScopedAStatus getFrequencyMinimum(float* freqMinimumHz) override;
    ndk::ScopedAStatus getBandwidthAmplitudeMap(std::vector<float>* _aidl_return) override;
    ndk::ScopedAStatus getPwlePrimitiveDurationMax(int32_t* durationMs) override;
    ndk::ScopedAStatus getPwleCompositionSizeMax(int32_t* maxSize) override;
    ndk::ScopedAStatus getSupportedBraking(std::vector<Braking>* supported) override;
    ndk::ScopedAStatus composePwle(const std::vector<PrimitivePwle>& composite,
                                   const std::shared_ptr<IVibratorCallback>& callback) override;

private:
    bool openInputLocked();
    int setGainLocked(uint8_t gainPct);
    int playWaveformLocked(uint16_t waveformIndex, int32_t timeoutMs, uint8_t gainPct);
    int playSineLocked(int32_t timeoutMs, uint8_t level);
    int playHapticLocked(uint16_t waveformIndex, int32_t timeoutMs, uint8_t gainPct);
    int eraseEffectLocked();
    void notifyOnComplete(const std::shared_ptr<IVibratorCallback>& callback, int32_t delayMs,
                          uint64_t generation);
    int32_t durationForEffect(Effect effect) const;
    int32_t durationForPrimitive(CompositePrimitive primitive) const;
    uint8_t gainForEffect(Effect effect, EffectStrength strength) const;
    uint8_t gainForPrimitive(CompositePrimitive primitive, float scale) const;
    uint16_t waveformForEffect(Effect effect) const;
    uint16_t waveformForPrimitive(CompositePrimitive primitive) const;
    void scheduleFollowupHaptic(uint64_t generation, int32_t delayMs, uint16_t waveformIndex,
                                int32_t durationMs, uint8_t gainPct);

    std::mutex mLock;
    int mFd = -1;
    int16_t mCurrentEffect = -1;
    std::string mInputPath;
    std::string mInputName;
    bool mHasCustom = false;
    bool mHasGain = false;
    std::atomic<uint64_t> mGeneration{0};
};

}  // namespace vibrator
}  // namespace hardware
}  // namespace android
}  // namespace aidl
