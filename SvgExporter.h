/*
SvgExporter provides simple interface to create and export SVG v1.1 images.

(c) 2018 David Kutak
*/

#pragma once
#include <iostream>
#include <vector>
#include <utility>

namespace protolib
{
	class SvgExporter
	{
	private:
		size_t mWidth;
		size_t mHeight;
		size_t mDefaultStrokeWidth;
		std::string mDefaultFill;
		std::string mDefaultStroke;

		std::vector<std::string> mSvgObjects;
	public:
		/**
		Main constructor to initialize the whole class

		@param width width of an image (if set to 0, automatically determined will be used)
		@param height height of an image (if set to 0, automatically determined will be used)
		@param defaultFill default fill color
		@param defaultStroke default stroke color
		@param defaultStrokeWidth default width of a stroke
		*/
		SvgExporter(size_t width = 0, size_t height = 0, 
			const std::string& defaultFill = "red", const std::string& defaultStroke = "black", 
			size_t defaultStrokeWidth = 2)
			:mWidth(width), mHeight(height), mDefaultFill(defaultFill), 
			mDefaultStroke(defaultStroke), mDefaultStrokeWidth(defaultStrokeWidth)
		{ }

		/**
		Returns width of an image

		@return width of an image
		*/
		size_t getWidth() const
		{
			return mWidth;
		}

		/**
		Sets width of an image

		@param width new width
		*/
		void setWidth(size_t width)
		{
			mWidth = width;
		}

		/**
		Returns height of an image

		@return height of an image
		*/
		size_t getHeight() const
		{
			return mHeight;
		}

		/**
		Sets height of an image

		@param height new height
		*/
		void setHeight(size_t height)
		{
			mHeight = height;
		}

		/**
		Returns default stroke width

		@return default stroke width
		*/
		size_t getDefaultStrokeWidth() const
		{
			return mDefaultStrokeWidth;
		}

		/**
		Sets default stroke width

		@param strokeWidth new default stroke width
		*/
		void setDefaultStrokeWidth(size_t strokeWidth)
		{
			mDefaultStrokeWidth = strokeWidth;
		}

		/**
		Returns default fill color

		@return default fill color
		*/
		const std::string& getDefaultFillColor() const
		{
			return mDefaultFill;
		}

		/**
		Sets default fill color

		@param color new default fill color
		*/
		void setDefaultFillColor(const std::string& color)
		{
			mDefaultFill = color;
		}

		/**
		Returns default stroke color

		@return default stroke color
		*/
		const std::string& getDefaultStrokeColor() const
		{
			return mDefaultStroke;
		}

		/**
		Sets default stroke color

		@param color new default stroke color
		*/
		void setDefaultStrokeColor(const std::string& color)
		{
			mDefaultStroke = color;
		}

		/**
		Returns list of current SVG elements as a string

		@return std::vector with SVG objects in a string format
		*/
		const std::vector<std::string>& getSvgObjects() const
		{
			return mSvgObjects;
		}

		/**
		Saves SVG image to file

		@param filepath file name
		*/
		void save(const std::string& filepath) const;

		/**
		Clears all currently held SVG elements
		*/
		void removeAll();

		/**
		Removes last inserted SVG element
		*/
		void removeLast();

		/**
		Adds rectangle with given parameters

		@param x x-coordinate
		@param y y-coordinate
		@param width rectangle width
		@param height rectangle height
		@param fill fill color (if empty, then default is used)
		@param stroke stroke color (if empty, then default is used)
		@param additionalAttributes additional SVG attributes
		*/
		void addRectangle(int x, int y, int width, int height, 
			const std::string& fill = std::string(), const std::string& stroke = std::string(), 
			const std::string& additionalAttributes = std::string());

		/**
		Adds circle with given parameters

		@param x x-coordinate
		@param y y-coordinate
		@param radius circle radius
		@param fill fill color (if empty, then default is used)
		@param stroke stroke color (if empty, then default is used)
		@param additionalAttributes additional SVG attributes
		*/
		void addCircle(int x, int y, int radius, 
			const std::string& fill = std::string(), const std::string& stroke = std::string(),
			const std::string& additionalAttributes = std::string());

		/**
		Adds ellipse with given parameters

		@param x x-coordinate
		@param y y-coordinate
		@param width ellipse width
		@param height ellipse height
		@param fill fill color (if empty, then default is used)
		@param stroke stroke color (if empty, then default is used)
		@param additionalAttributes additional SVG attributes
		*/
		void addEllipse(int x, int y, int width, int height,
			const std::string& fill = std::string(), const std::string& stroke = std::string(),
			const std::string& additionalAttributes = std::string());

		/**
		Adds line with given parameters

		@param xStart x-coordinate of start point
		@param yStart y-coordinate of start point
		@param xEnd x-coordinate of end point
		@param yEnd y-coordinate of end point
		@param stroke stroke color (if empty, then default is used)
		@param additionalAttributes additional SVG attributes
		*/
		void addLine(int xStart, int yStart, int xEnd, int yEnd,
			const std::string& stroke = std::string(),
			const std::string& additionalAttributes = std::string());

		/**
		Adds polygon with given parameters

		@param points std::vector of polygon vertices (integer pairs representing x,y coordinates)
		@param fill fill color (if empty, then default is used)
		@param stroke stroke color (if empty, then default is used)
		@param additionalAttributes additional SVG attributes
		*/
		void addPolygon(const std::vector<std::pair<int, int>>& points,
			const std::string& fill = std::string(), const std::string& stroke = std::string(),
			const std::string& additionalAttributes = std::string());

		/**
		Adds polyline with given parameters

		@param points std::vector of polyline vertices (integer pairs representing x,y coordinates)
		@param stroke stroke color (if empty, then default is used)
		@param additionalAttributes additional SVG attributes
		*/
		void addPolyline(const std::vector<std::pair<int, int>>& points,
			const std::string& stroke = std::string(),
			const std::string& additionalAttributes = std::string());

		/**
		Adds text with given parameters and content

		@param x x-coordinate
		@param y y-coordinate
		@param fontSize font size
		@param text text content
		@param fill fill color (if empty, then default is used)
		@param stroke stroke color (if empty, then default is used)
		@param additionalAttributes additional SVG attributes
		*/
		void addText(int x, int y, int fontSize, const std::string& text,
			const std::string& fill = std::string(), const std::string& stroke = std::string(),
			const std::string& additionalAttributes = std::string());

	};
}

