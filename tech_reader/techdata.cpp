#include "tech_parser.hpp"
#include "techscanner.h"
#include "techdata.h"

namespace tech {
	TechData::TechData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false)
	{
		planeList.clear();

		std::ifstream input;
		std::string stdfilename = filename.toStdString();

		streamname = filename;
		input.open(stdfilename, std::ios::in);

		lexer = new TechScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new TechParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	TechScanner *TechData::getLexer()
	{
		return lexer;
	}

	QStringList TechData::getPlanes()
	{
		return planeList;
	}

	QStringList TechData::getTypeNames()
	{
		return typeList.keys();
	}

	QStringList TechData::getStyleNames()
	{
		return styleList.keys();
	}

	void TechData::addPlane(std::string s)
	{
		planeList.append(QString::fromStdString(s));
	}

	void TechData::addType(std::string s1, std::string s2)
	{
		QString typeName = QString::fromStdString(s1);
		QString value = QString::fromStdString(s2);
		typeList[typeName].append(value);
	}

	void TechData::addCIFMapping(int i, std::string layer)
	{
		cifLayerNames[i] = QString::fromStdString(layer);
	}

	QString TechData::layerNameFromCIF(int i)
	{
		if(cifLayerNames.contains(i)) return cifLayerNames[i];
		return QString();
	}

	QStringList TechData::getType(QString s)
	{
		return typeList[s];
	}

	QStringList TechData::getDesignRules()
	{
		return designRules.keys();
	}

	TechDesignRule TechData::getDesignRule(QString n) {
		if(designRules.contains(n))
			return designRules[n];
		return TechDesignRule();
	}

	void TechData::addDesignRuleSpacing(std::string n1, std::string n2, int d, std::string s, std::string m)
	{
		QStringList nl;
		QString material1;
		QString material2;
		QString material;
		QString severity;
		bool touching_ok;
		TechDesignRule rule;

		material1 = QString::fromStdString(n1);
		material2 = QString::fromStdString(n2);
		severity = QString::fromStdString(s);
		nl = material1.split(',');

		foreach(material, nl) {
			if(designRules.contains(material))
				rule = designRules[material];

			touching_ok = (severity=="touching_ok");
			rule.setName(material);
			rule.setSpacing(material2, d, QString::fromStdString(m), touching_ok);

			designRules[material] = rule;
		}
	}

	void TechData::addDesignRuleWidth(std::string n, int w, std::string m)
	{
		QStringList nl;
		QString name;
		QString material;
		TechDesignRule rule;

		name = QString::fromStdString(n);
		nl = name.split(',');

		foreach(material,nl) {
			if(designRules.contains(material))
				rule = designRules[material];

			rule.setName(material);
			rule.setMinimumWidth(w);
			rule.setWidthMessage(QString::fromStdString(m));

			designRules[material]=rule;
		}
	}

	void TechData::addPlaneOrder(int i, std::string plane)
	{
		QString n = QString::fromStdString(plane);
		planeOrder[n] = i;
	}

	int TechData::getNumPlanes()
	{
		return planeOrder.count();
	}

	int TechData::getPlaneOrder(QString n)
	{
		int ret = 0;
		if(planeOrder.contains(n)) {
			ret = planeOrder[n];
		}
		return ret;
	}

	QMap<QString,QVector<int>> TechData::getStyle(QString s)
	{
		return styleList[s];
	}

	void TechData::setRecentStyle(std::string s)
	{
		recentStyle = QString::fromStdString(s);
	}

	void TechData::addToRecentStyleMember(int i)
	{
		styleList[recentStyle][recentStyleMember].append(i);
	}

	void TechData::setRecentStyleMember(std::string s)
	{
		recentStyleMember = QString::fromStdString(s);
	}
}
