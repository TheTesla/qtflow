#ifndef MAGICDATA_H
#define MAGICDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>

#include "magicscanner.h"

typedef QVector<QRect> rects_t;
typedef QMap<QString,rects_t> layers_t;

namespace magic {
	class MagicData
	{
	public:
		MagicData();
		MagicData(QString file);

		bool loadFile(QString);

		// building the info
		void clearParsedElements();
		void addRectangle(int x, int y, int w, int h);
		void setLayer(std::string *s);
		class MagicScanner *getLexer();

	private:
		rects_t parsedElements;
		class MagicScanner* lexer;
		class MagicParser* parser;
		QString title;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;
	};
}

#endif // MAGICDATA_H