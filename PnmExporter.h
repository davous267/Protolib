/*
PnmExporter provides simple interface to create and export PNM images.
Each of the PPM, PGM and PBM file formats is supported. 

(c) 2018 David Kutak
*/

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <algorithm>

namespace protolib
{
	enum class PnmImageType
	{
		PBM_ASCII = 1,	// Binary image saved as an ASCII data
		PGM_ASCII,		// Grayscale image saved as an ASCII data
		PPM_ASCII,		// RGB image saved as an ASCII data
		PBM_BIN,		// Binary image saved as a binary data
		PGM_BIN,		// Grayscale image saved as a binary data
		PPM_BIN			// RGB image saved as a binary data
	};

	template<typename TChannelType = uint8_t>
	class PnmExporter
	{
	public:
		struct PixelColor
		{
			// In case of RGB image, these variables are used ...
			TChannelType r;
			TChannelType g;
			TChannelType b;

			// ... otherwise this variable should be used
			TChannelType y;

			PixelColor(TChannelType ry = 0, TChannelType g = 0, TChannelType b = 0)
				:r(ry), y(ry), g(g), b(b)
			{ }
		};
	private:
		PnmImageType mImageType;
		size_t mWidth;
		size_t mHeight;
		std::vector<TChannelType> mPixelData;

		std::string getMagicNumber() const
		{
			return "P" + std::to_string(static_cast<int>(mImageType));
		}

		bool isBinFormat() const
		{
			return static_cast<int>(mImageType) > 3;
		}

		void resizePixelData()
		{
			mPixelData.resize(mWidth * mHeight * getNumberOfChannels());
		}

		void saveAscii(std::ofstream& output)
		{
			size_t valuesPerRow = mWidth * getNumberOfChannels();

			for (size_t i = 0; i < mPixelData.size(); ++i)
			{
				output << static_cast<unsigned>(mPixelData[i]) << " ";
				if ((i + 1) % (valuesPerRow) == 0)
				{
					output << std::endl;
				}
			}
		}

		void saveBinary(std::ofstream& output)
		{
			// PBM stores data in a "pixel per bit" format so I have
			// to transform pixel data before saving them to file
			if (mImageType == PnmImageType::PBM_BIN)
			{
				std::vector<uint8_t> binData((mWidth / 8 + (mWidth % 8 == 0 ? 0 : 1)) * mHeight);

				for (size_t y = 0, bdPos = 0; y < mHeight; ++y)
				{
					for (size_t x = 0; x < mWidth; x += 8, ++bdPos)
					{
						size_t upperBound = mWidth - x > 8 ? 8 : mWidth - x;
						for (size_t k = 0; k < upperBound; ++k)
						{
							// mPixelData equals only to 1 or 0 in case of PBM
							binData[bdPos] |= mPixelData[y * mWidth + x + k] << (upperBound - 1 - k);
						}
					}
				}

				output.write(reinterpret_cast<const char*>(binData.data()),
					binData.size() * sizeof(uint8_t));
			}
			else
			{
				output.write(reinterpret_cast<const char*>(mPixelData.data()),
					mPixelData.size() * sizeof(TChannelType));
			}
		}
	public:
		/**
		Main constructor to initialize the whole class

		@param width width of an image
		@param height height of an image
		@param type image type
		*/
		PnmExporter(size_t width, size_t height, PnmImageType type = PnmImageType::PPM_BIN)
			:mWidth(width), mHeight(height), mImageType(type)
		{ 
			resizePixelData();
		}

		/**
		Returns width of an image (in pixels)

		@return width of an image
		*/
		size_t getWidth() const
		{
			return mWidth;
		}

		/**
		Returns height of an image (in pixels)

		@return height of an image
		*/
		size_t getHeight() const
		{
			return mHeight;
		}

		/**
		Returns image type

		@return image type
		*/
		PnmImageType getImageType() const
		{
			return mImageType;
		}

		/**
		Returns raw pixel data

		@return vector with pixel data
		*/
		const std::vector<TChannelType>& getPixelData() const
		{
			return mPixelData;
		}

		/**
		Sets new width of an image

		@param width new width
		*/
		void setWidth(size_t width)
		{
			mWidth = width;
			resizePixelData();
		}

		/**
		Sets new height of an image

		@param height new height
		*/
		void setHeight(size_t height)
		{
			mHeight = height;
			resizePixelData();
		}

		/**
		Replaces current pixel data with new one
		Pixel data vector is then resized according to the image
		dimensions so if the new data are not of the same size
		the output might be not be as expected

		@param width new width
		*/
		void setPixelData(const std::vector<TChannelType>& pixelData)
		{
			mPixelData = pixelData;
			resizePixelData();
		}

		/**
		Returns number of image channels

		@return number of channels
		*/
		size_t getNumberOfChannels() const
		{
			return (mImageType == PnmImageType::PPM_ASCII ||
					mImageType == PnmImageType::PPM_BIN) ? 3 : 1;
		}

		/**
		Swaps between ASCII and binary data representation
		*/
		void swapBtwnASCIIandBIN()
		{
			int enumPos = static_cast<int>(mImageType);
			mImageType = static_cast<PnmImageType>(enumPos + (isBinFormat() ? -3 : 3));
		}

		/**
		Sets value of all pixels to default-constructed TChannelType type
		(the expected value is typically zero)
		*/
		void clear()
		{
			std::fill(mPixelData.begin(), mPixelData.end(), TChannelType());
		}

		/**
		Gets color of pixel at given coordinates

		@param x x-coordinate
		@param y y-coordinate
		@return color of pixel (x,y)
		*/
		PixelColor getPixel(size_t x, size_t y)
		{
			PixelColor result;
			if (getNumberOfChannels() == 3)
			{
				size_t startPos = 3 * (y * mWidth + x);

				// Assignments are in this order to make just one boundary check
				// by calling at() function
				result.b = mPixelData.at(startPos + 2);
				result.r = mPixelData[startPos];
				result.g = mPixelData[startPos + 1];
			}
			else
			{
				result.y = mPixelData.at(y * mWidth + x);
			}
			
			return result;
		}

		/**
		Sets color of pixel at given coordinates

		@param x x-coordinate
		@param y y-coordinate
		@param color new color of pixel (x,y)
		*/
		void setPixel(size_t x, size_t y, const PixelColor& color)
		{
			if (getNumberOfChannels() == 3)
			{
				size_t startPos = 3 * (y * mWidth + x);
				mPixelData.at(startPos + 2) = color.b;
				mPixelData[startPos] = color.r;
				mPixelData[startPos + 1] = color.g;
			}
			else
			{
				mPixelData.at(y * mWidth + x) = ((mImageType == PnmImageType::PBM_ASCII ||
					mImageType == PnmImageType::PBM_BIN) && color.y > 1) ? 1 :color.y;
			}
		}

		/**
		Sets all pixels to given color

		@param color new color
		*/
		void setPixels(const PixelColor& color)
		{
			if (getNumberOfChannels() == 3)
			{
				for (size_t i = 0; i < mPixelData.size(); ++i)
				{
					if (i % 3 == 0)
					{
						mPixelData[i] = color.r;
					}
					else if (i % 3 == 1)
					{
						mPixelData[i] = color.g;
					}
					else
					{
						mPixelData[i] = color.b;
					}
				}
			}
			else
			{
				std::fill(mPixelData.begin(), mPixelData.end(), color.y);
			}
		}

		/**
		This function, given a predicate pred(x,y), 
		sets color of each pixel (x,y) fulfilling pred(x,y) to "color"
		
		@param pred binary predicate
		@color color new color for pixels fulfilling predicate
		*/
		template<typename Pred>
		void setPixels(Pred pred, const PixelColor& color)
		{
			for (size_t y = 0; y < mHeight; ++y)
			{
				for (size_t x = 0; x < mWidth; ++x)
				{
					if (pred(x, y))
					{
						setPixel(x, y, color);
					}
				}
			}
		}

		/**
		Draws a circle with given parameters

		@param cx x-coordinate of center
		@param cy y-coordinate of center
		@param radius circle radius
		@param color circle color
		*/
		void addCircle(int cx, int cy, int radius, const PixelColor& color)
		{
			setPixels([=](int x, int y) { 
				return (x - cx)*(x - cx) + (y - cy) * (y - cy) < radius * radius; 
			}, color);
		}

		/**
		Draws a rectangle with given parameters

		@param xLeft x-coordinate of top left point
		@param yTop y-coordinate of top left point
		@param width rectangle width
		@param height rectangle height
		@param color rectangle color
		*/
		void addRectangle(int xLeft, int yTop, int width, int height, const PixelColor& color)
		{
			int xRight = xLeft + width;
			int yBottom = yTop + height;

			setPixels([=](int x, int y) {
				return x >= xLeft && x <= xRight && y >= yTop && y <= yBottom;
			}, color);
		}

		/**
		Saves an image to file

		@param fileName name of the file (including extension, i.e. *.ppm/pgm/pbm)
		*/
		void save(const std::string& fileName)
		{
			std::ofstream output(fileName, isBinFormat() ? std::ios::binary : std::ios::out);

			// Print header
			output << getMagicNumber() << std::endl;
			output << mWidth << " " << mHeight << std::endl;
			if (mImageType != PnmImageType::PBM_ASCII &&
				mImageType != PnmImageType::PBM_BIN)
			{
				output << static_cast<unsigned>(std::numeric_limits<TChannelType>::max()) << std::endl;
			}
			
			// Print data
			if (!isBinFormat())
			{
				saveAscii(output);
			}
			else
			{
				saveBinary(output);
			}
		}
	};
}