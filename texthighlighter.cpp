#include "texthighlighter.h"

TextHighlighter::TextHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

        keywordFormat.setForeground(Qt::blue);
        keywordFormat.setFontWeight(QFont::Bold);
        const QString keywordPatterns[] = {
            QStringLiteral("\\bchar\\b"), QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconst\\b"),
            QStringLiteral("\\bdouble\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bexplicit\\b"),
            QStringLiteral("\\bfriend\\b"), QStringLiteral("\\binline\\b"), QStringLiteral("\\bint\\b"),
            QStringLiteral("\\blong\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\boperator\\b"),
            QStringLiteral("\\bprivate\\b"), QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"),
            QStringLiteral("\\bshort\\b"), QStringLiteral("\\bsignals\\b"), QStringLiteral("\\bsigned\\b"),
            QStringLiteral("\\bslots\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstruct\\b"),
            QStringLiteral("\\btemplate\\b"), QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypename\\b"),
            QStringLiteral("\\bunion\\b"), QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\bvirtual\\b"),
            QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bbool\\b"), QStringLiteral("\\bfloat\\b"),
            QStringLiteral("\\bfor\\b"), QStringLiteral("\\bif\\b"),

            QStringLiteral("\\buniform\\b"), QStringLiteral("\\bvec2\\b"), QStringLiteral("\\bvec3\\b"),
            QStringLiteral("\\bvec4\\b"), QStringLiteral("\\bin\\b"), QStringLiteral("\\bout\\b"),
            QStringLiteral("\\bsamplerExternalOES\\b")
        };

        for (const QString &pattern : keywordPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }


        preprocessorLineFormat.setForeground(Qt::blue);
        preprocessorLineFormat.setFontWeight(QFont::Bold);
        rule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
        rule.format = preprocessorLineFormat;
        highlightingRules.append(rule);

        //classFormat.setFontWeight(QFont::Bold);
        //classFormat.setForeground(Qt::darkMagenta);
        //rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
        //rule.format = classFormat;
        //highlightingRules.append(rule);

        quotationFormat.setForeground(Qt::darkYellow);
        rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        functionFormat.setForeground(Qt::darkCyan);
        functionFormat.setFontWeight(QFont::Bold);
        rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
        rule.format = functionFormat;
        highlightingRules.append(rule);

        singleLineCommentFormat.setForeground(Qt::darkGreen);
        rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(Qt::darkGreen);

        commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void TextHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
            QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex
                                + match.capturedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
}
