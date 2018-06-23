#include "SvgExporter.h"
#include <fstream>
#include <string>

namespace protolib
{
	void SvgExporter::save(const std::string& filepath) const
	{
		std::ofstream svgImage(filepath);

		svgImage << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
		svgImage << "<svg ";
		if (mWidth > 0 && mHeight > 0)
		{
			svgImage << "width=\"" << mWidth << "\" height=\"" << mHeight << "\" ";
		}
		svgImage << "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">" << std::endl;

		for (const auto& object : mSvgObjects)
		{
			svgImage << "\t" << object << std::endl;
		}

		svgImage << "</svg>" << std::endl;
	}

	void SvgExporter::removeAll()
	{
		mSvgObjects.clear();
	}

	void SvgExporter::removeLast()
	{
		if (!mSvgObjects.empty()) { mSvgObjects.pop_back(); }
	}

	void SvgExporter::addRectangle(int x, int y, int width, int height, 
		const std::string& fill, const std::string& stroke, const std::string& additionalAttributes)
	{
		mSvgObjects.push_back("<rect x=\"" + std::to_string(x) + "\" y=\"" + std::to_string(y) + 
			"\" width=\"" + std::to_string(width) + "\" height=\"" + std::to_string(height) + 
			(mDefaultStrokeWidth > 0 ? "\" stroke=\"" + (stroke.empty() ? mDefaultStroke : stroke) : "") +
			"\" fill=\"" + (fill.empty() ? mDefaultFill : fill) +
			(mDefaultStrokeWidth > 0 ? ("\" stroke-width=\"" + std::to_string(mDefaultStrokeWidth)) : "") + 
			"\" " + additionalAttributes + " />"
		);
	}

	void SvgExporter::addCircle(int x, int y, int radius, 
		const std::string& fill, const std::string& stroke, const std::string& additionalAttributes)
	{
		mSvgObjects.push_back("<circle cx=\"" + std::to_string(x) + "\" cy=\"" + std::to_string(y) +
			"\" r=\"" + std::to_string(radius) +
			(mDefaultStrokeWidth > 0 ? "\" stroke=\"" + (stroke.empty() ? mDefaultStroke : stroke) : "") +
			"\" fill=\"" + (fill.empty() ? mDefaultFill : fill) +
			(mDefaultStrokeWidth > 0 ? ("\" stroke-width=\"" + std::to_string(mDefaultStrokeWidth)) : "") +
			"\" " + additionalAttributes + " />"
		);
	}

	void SvgExporter::addEllipse(int x, int y, int width, int height, 
		const std::string& fill, const std::string& stroke, const std::string& additionalAttributes)
	{
		mSvgObjects.push_back("<ellipse cx=\"" + std::to_string(x) + "\" cy=\"" + std::to_string(y) +
			"\" rx=\"" + std::to_string(width) + "\" ry=\"" + std::to_string(height) +
			(mDefaultStrokeWidth > 0 ? "\" stroke=\"" + (stroke.empty() ? mDefaultStroke : stroke) : "") +
			"\" fill=\"" + (fill.empty() ? mDefaultFill : fill) +
			(mDefaultStrokeWidth > 0 ? ("\" stroke-width=\"" + std::to_string(mDefaultStrokeWidth)) : "") +
			"\" " + additionalAttributes + " />"
		);
	}

	void SvgExporter::addLine(int xStart, int yStart, int xEnd, int yEnd, 
		const std::string& stroke, const std::string& additionalAttributes)
	{
		mSvgObjects.push_back("<line x1=\"" + std::to_string(xStart) + "\" y1=\"" + std::to_string(yStart) +
			"\" x2=\"" + std::to_string(xEnd) + "\" y2=\"" + std::to_string(yEnd) +
			(mDefaultStrokeWidth > 0 ? "\" stroke=\"" + (stroke.empty() ? mDefaultStroke : stroke) : "") +
			(mDefaultStrokeWidth > 0 ? ("\" stroke-width=\"" + std::to_string(mDefaultStrokeWidth)) : "") +
			"\" " + additionalAttributes + " />"
		);
	}

	void SvgExporter::addPolygon(const std::vector<std::pair<int, int>>& points, 
		const std::string& fill, const std::string& stroke, const std::string& additionalAttributes)
	{
		std::string pointsStr;
		for (const auto& coord : points)
		{
			pointsStr += std::to_string(coord.first) + "," + std::to_string(coord.second) + " ";
		}

		mSvgObjects.push_back("<polygon points=\"" + pointsStr +
			(mDefaultStrokeWidth > 0 ? "\" stroke=\"" + (stroke.empty() ? mDefaultStroke : stroke) : "") +
			"\" fill=\"" + (fill.empty() ? mDefaultFill : fill) +
			(mDefaultStrokeWidth > 0 ? ("\" stroke-width=\"" + std::to_string(mDefaultStrokeWidth)) : "") +
			"\" " + additionalAttributes + " />"
		);
	}

	void SvgExporter::addPolyline(const std::vector<std::pair<int, int>>& points,
		const std::string& stroke, const std::string& additionalAttributes)
	{
		std::string pointsStr;
		for (const auto& coord : points)
		{
			pointsStr += std::to_string(coord.first) + "," + std::to_string(coord.second) + " ";
		}

		mSvgObjects.push_back("<polyline points=\"" + pointsStr +
			(mDefaultStrokeWidth > 0 ? "\" stroke=\"" + (stroke.empty() ? mDefaultStroke : stroke) : "") +
			(mDefaultStrokeWidth > 0 ? ("\" stroke-width=\"" + std::to_string(mDefaultStrokeWidth)) : "") +
			"\" " + additionalAttributes + " />"
		);
	}

	void SvgExporter::addText(int x, int y, int fontSize, const std::string& text,
		const std::string& fill, const std::string& stroke, const std::string& additionalAttributes)
	{
		mSvgObjects.push_back("<text x=\"" + std::to_string(x) + "\" y=\"" + std::to_string(y) +
			"\" font-size=\"" + std::to_string(fontSize) + 
			(mDefaultStrokeWidth > 0 ? "\" stroke=\"" + (stroke.empty() ? mDefaultStroke : stroke) : "") +
			"\" fill=\"" + (fill.empty() ? mDefaultFill : fill) +
			(mDefaultStrokeWidth > 0 ? ("\" stroke-width=\"" + std::to_string(mDefaultStrokeWidth)) : "") +
			"\" " + additionalAttributes + ">" + text + "</text>"
		);
	}
}
