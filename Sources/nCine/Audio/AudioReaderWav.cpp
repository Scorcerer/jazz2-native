#include "AudioReaderWav.h"
#include "AudioLoaderWav.h"
#include "../../Main.h"

#include <cstring>

#include <IO/FileStream.h>

using namespace Death::IO;

namespace nCine
{
	AudioReaderWav::AudioReaderWav(std::unique_ptr<Stream> fileHandle)
		: fileHandle_(std::move(fileHandle))
	{
		ASSERT(fileHandle_->IsValid());
	}

	unsigned long int AudioReaderWav::read(void* buffer, unsigned long int bufferSize) const
	{
		ASSERT(buffer);
		ASSERT(bufferSize > 0);

		unsigned long int bytes = 0;
		unsigned long int bufferSeek = 0;

		do {
			// Read up to a buffer's worth of decoded sound data
			bytes = fileHandle_->Read(buffer, bufferSize);
			bufferSeek += bytes;
		} while (bytes > 0 && bufferSize - bufferSeek > 0);

		return bufferSeek;
	}

	void AudioReaderWav::rewind() const
	{
		fileHandle_->Seek(AudioLoaderWav::HeaderSize, SeekOrigin::Begin);
	}
}
