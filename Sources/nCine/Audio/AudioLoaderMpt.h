#pragma once

#include "AudioReaderMpt.h"

#if defined(WITH_OPENMPT) || defined(DOXYGEN_GENERATING_OUTPUT)

#include "IAudioLoader.h"

namespace nCine
{
	/// Module audio loader using `libopenmpt` library
	class AudioLoaderMpt : public IAudioLoader
	{
	public:
		explicit AudioLoaderMpt(std::unique_ptr<Death::IO::Stream> fileHandle);

		std::unique_ptr<IAudioReader> createReader() override;
	};
}

#endif