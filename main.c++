#ifdef Q_OS_WIN
#include <windows.h>
#include <shlobj.h>
#endif

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QTextBrowser>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>
#include <QFrame>
#include <QMenu>
#include <QMenuBar>
#include <QDialog>
#include <QSpinBox>
#include <QComboBox>
#include <QFontComboBox>
#include <QTabWidget>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QScrollBar>
#include <QShortcut>
#include <QSettings>
#include <QTimer>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextOption>
#include <QFontDatabase>
#include <QStyleHints>
#include <QPrintDialog>
#include <QPrinter>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <cmath>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

struct ThemePalette {
    QString bg_main;
    QString bg_secondary;
    QString bg_tertiary;
    QString bg_hover;
    QString bg_selected;
    QString border;
    QString border_focus;
    QString text_primary;
    QString text_secondary;
    QString text_dim;
    QString accent;
    QString accent_hover;
    QString accent_bg;
    QString menu_highlight;
    QString menu_text_highlight;
    QString danger;
    QString warning;
    QString success;
    QString editor_bg;
    QString editor_gutter;
    QString editor_current_line;
    QString preview_bg;
    QString preview_code_bg;
    QString preview_border;
};

static const ThemePalette DARK_PALETTE = {
    "#0f111a", "#151824", "#1a1d2c", "#222638", "#2c324b",
    "#25293d", "#6366f1", "#f1f5f9", "#94a3b8", "#64748b",
    "#6366f1", "#818cf8", "rgba(99, 102, 241, 0.22)", "#4f46e5", "#ffffff",
    "#f43f5e", "#fbbf24", "#10b981", "#10121b", "#0b0d14",
    "#191d2d", "#10121b", "#0c0e16", "#25293d"
};

static const ThemePalette LIGHT_PALETTE = {
    "#f8fafc", "#f1f5f9", "#ffffff", "#e2e8f0", "#cbd5e1",
    "#cbd5e1", "#4f46e5", "#0f172a", "#475569", "#94a3b8",
    "#4f46e5", "#4338ca", "rgba(79, 70, 229, 0.14)", "#4f46e5", "#ffffff",
    "#e11d48", "#d97706", "#059669", "#ffffff", "#f8fafc",
    "#f1f5f9", "#ffffff", "#f8fafc", "#e2e8f0"
};

inline const ThemePalette& getPalette(const QString &theme) {
    return (theme == "light") ? LIGHT_PALETTE : DARK_PALETTE;
}

QString detectSystemTheme() {
#ifdef Q_OS_WIN
    HKEY hKey;
    LONG res = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        0, KEY_READ, &hKey
    );
    if (res == ERROR_SUCCESS) {
        DWORD value = 1;
        DWORD size = sizeof(value);
        if (RegQueryValueExW(hKey, L"AppsUseLightTheme", nullptr, nullptr, (LPBYTE)&value, &size) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return (value == 1) ? "light" : "dark";
        }
        RegCloseKey(hKey);
    }
#endif

    if (QApplication::styleHints()) {
        auto cs = QApplication::styleHints()->colorScheme();
        if (cs == Qt::ColorScheme::Dark) return "dark";
        if (cs == Qt::ColorScheme::Light) return "light";
    }

    if (qApp) {
        QColor col = qApp->palette().color(QPalette::Window);
        return (col.lightness() > 128) ? "light" : "dark";
    }
    return "dark";
}

QPixmap createMdReaderAppPixmap(int size = 256) {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

    const qreal s = size / 256.0;

    // 1. Base Drop Shadow
    QPainterPath shadowPath;
    shadowPath.addRoundedRect(QRectF(14 * s, 18 * s, 228 * s, 228 * s), 56 * s, 56 * s);
    painter.fillPath(shadowPath, QColor(2, 6, 23, 110));

    // 2. Main Sapphire Squircle Tile
    QPainterPath tilePath;
    tilePath.addRoundedRect(QRectF(14 * s, 14 * s, 228 * s, 228 * s), 54 * s, 54 * s);

    QLinearGradient tileGrad(QPointF(20 * s, 14 * s), QPointF(236 * s, 242 * s));
    tileGrad.setColorAt(0.0, QColor("#1e3a8a"));
    tileGrad.setColorAt(0.35, QColor("#172554"));
    tileGrad.setColorAt(0.85, QColor("#090d1f"));
    tileGrad.setColorAt(1.0, QColor("#030712"));
    painter.fillPath(tilePath, tileGrad);

    // Rim Glow
    QRadialGradient glowGrad(QPointF(128 * s, 36 * s), 130 * s);
    glowGrad.setColorAt(0.0, QColor(99, 102, 241, 100));
    glowGrad.setColorAt(0.7, QColor(59, 130, 246, 30));
    glowGrad.setColorAt(1.0, QColor(0, 0, 0, 0));
    painter.fillPath(tilePath, glowGrad);

    // Neon Border Outline
    QPen borderPen(QColor(96, 165, 250, 160));
    borderPen.setWidthF(2.2 * s);
    painter.setPen(borderPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(tilePath);

    // 3. Open Booklet 3D Backing
    QPainterPath bookBase;
    bookBase.addRoundedRect(QRectF(40 * s, 44 * s, 176 * s, 164 * s), 14 * s, 14 * s);
    painter.fillPath(bookBase, QColor("#1e40af"));

    // Left Page: Markdown Source Code
    QPainterPath leftPage;
    leftPage.moveTo(42 * s, 48 * s);
    leftPage.lineTo(124 * s, 54 * s);
    leftPage.lineTo(124 * s, 202 * s);
    leftPage.lineTo(44 * s, 196 * s);
    leftPage.quadTo(40 * s, 196 * s, 40 * s, 190 * s);
    leftPage.lineTo(40 * s, 56 * s);
    leftPage.quadTo(40 * s, 48 * s, 48 * s, 48 * s);
    leftPage.closeSubpath();

    QLinearGradient leftGrad(QPointF(42 * s, 48 * s), QPointF(124 * s, 202 * s));
    leftGrad.setColorAt(0.0, QColor("#181a24"));
    leftGrad.setColorAt(1.0, QColor("#0d0f17"));
    painter.fillPath(leftPage, leftGrad);

    painter.setPen(QPen(QColor(51, 65, 85, 180), 1.2 * s));
    painter.drawPath(leftPage);

    // Left Page Heading '#'
    QFont fontHash("Segoe UI", qRound(26 * s), QFont::Black);
    painter.setFont(fontHash);
    painter.setPen(QColor("#60a5fa"));
    painter.drawText(QRectF(52 * s, 60 * s, 28 * s, 28 * s), Qt::AlignCenter, "#");

    // Code Line Capsules
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#38bdf8"));
    painter.drawRoundedRect(QRectF(54 * s, 94 * s, 28 * s, 6 * s), 3 * s, 3 * s);
    painter.setBrush(QColor("#64748b"));
    painter.drawRoundedRect(QRectF(86 * s, 94 * s, 18 * s, 6 * s), 3 * s, 3 * s);

    painter.setBrush(QColor("#60a5fa"));
    painter.drawRoundedRect(QRectF(54 * s, 107 * s, 40 * s, 6 * s), 3 * s, 3 * s);

    painter.setBrush(QColor("#94a3b8"));
    painter.drawRoundedRect(QRectF(54 * s, 120 * s, 24 * s, 6 * s), 3 * s, 3 * s);
    painter.setBrush(QColor("#a855f7"));
    painter.drawRoundedRect(QRectF(82 * s, 120 * s, 32 * s, 6 * s), 3 * s, 3 * s);

    // Triple backticks ```
    painter.setBrush(QColor("#cbd5e1"));
    for (int i = 0; i < 3; ++i) {
        painter.drawRoundedRect(QRectF((55 + i * 8) * s, 136 * s, 4.5 * s, 4.5 * s), 1.5 * s, 1.5 * s);
    }

    painter.setBrush(QColor("#38bdf8"));
    painter.drawRoundedRect(QRectF(54 * s, 149 * s, 36 * s, 6 * s), 3 * s, 3 * s);
    painter.setBrush(QColor("#818cf8"));
    painter.drawRoundedRect(QRectF(54 * s, 162 * s, 26 * s, 6 * s), 3 * s, 3 * s);
    painter.setBrush(QColor("#38bdf8"));
    painter.drawRoundedRect(QRectF(84 * s, 162 * s, 24 * s, 6 * s), 3 * s, 3 * s);
    painter.drawRoundedRect(QRectF(54 * s, 175 * s, 48 * s, 6 * s), 3 * s, 3 * s);

    // Spine Shadow
    QPainterPath spinePath;
    spinePath.moveTo(122 * s, 54 * s);
    spinePath.lineTo(128 * s, 54 * s);
    spinePath.lineTo(128 * s, 202 * s);
    spinePath.lineTo(122 * s, 202 * s);
    spinePath.closeSubpath();
    painter.fillPath(spinePath, QColor(10, 14, 26, 120));

    // Right Page: Preview Formatted Document
    QPainterPath rightPage;
    rightPage.moveTo(126 * s, 54 * s);
    rightPage.lineTo(206 * s, 44 * s);
    rightPage.quadTo(214 * s, 44 * s, 214 * s, 52 * s);
    rightPage.lineTo(214 * s, 186 * s);
    rightPage.quadTo(214 * s, 194 * s, 206 * s, 196 * s);
    rightPage.lineTo(126 * s, 202 * s);
    rightPage.closeSubpath();

    QLinearGradient rightGrad(QPointF(126 * s, 50 * s), QPointF(214 * s, 196 * s));
    rightGrad.setColorAt(0.0, QColor("#ffffff"));
    rightGrad.setColorAt(0.9, QColor("#f8fafc"));
    rightGrad.setColorAt(1.0, QColor("#e2e8f0"));
    painter.fillPath(rightPage, rightGrad);

    painter.setPen(QPen(QColor(203, 213, 225, 200), 1.2 * s));
    painter.drawPath(rightPage);

    // Right Page: Bold 'M ↓' Symbol
    QFont fontM("Segoe UI", qRound(28 * s), QFont::Black);
    painter.setFont(fontM);
    painter.setPen(QColor("#1e293b"));
    painter.drawText(QRectF(136 * s, 54 * s, 36 * s, 36 * s), Qt::AlignLeft | Qt::AlignVCenter, "M");

    QPen arrowPen(QColor("#1e293b"));
    arrowPen.setWidthF(4.8 * s);
    arrowPen.setCapStyle(Qt::RoundCap);
    arrowPen.setJoinStyle(Qt::MiterJoin);
    painter.setPen(arrowPen);
    const qreal arrowX = 184 * s;
    painter.drawLine(QPointF(arrowX, 62 * s), QPointF(arrowX, 82 * s));

    QPainterPath arrowHead;
    arrowHead.moveTo(arrowX - 7 * s, 77 * s);
    arrowHead.lineTo(arrowX, 85 * s);
    arrowHead.lineTo(arrowX + 7 * s, 77 * s);
    painter.drawPath(arrowHead);

    // Placeholders & Image Card
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#cbd5e1"));
    painter.drawRoundedRect(QRectF(136 * s, 98 * s, 64 * s, 5.5 * s), 2.5 * s, 2.5 * s);
    painter.drawRoundedRect(QRectF(136 * s, 109 * s, 52 * s, 5.5 * s), 2.5 * s, 2.5 * s);

    QPainterPath imgCard;
    imgCard.addRoundedRect(QRectF(136 * s, 126 * s, 38 * s, 34 * s), 4 * s, 4 * s);
    painter.fillPath(imgCard, QColor("#3b82f6"));

    painter.setBrush(QColor("#ffffff"));
    painter.drawEllipse(QRectF(160 * s, 131 * s, 6.5 * s, 6.5 * s));

    QPainterPath mtn;
    mtn.moveTo(136 * s, 160 * s);
    mtn.lineTo(146 * s, 144 * s);
    mtn.lineTo(156 * s, 153 * s);
    mtn.lineTo(164 * s, 142 * s);
    mtn.lineTo(174 * s, 160 * s);
    mtn.closeSubpath();
    painter.fillPath(mtn, QColor("#1d4ed8"));

    painter.setBrush(QColor("#cbd5e1"));
    painter.drawRoundedRect(QRectF(180 * s, 130 * s, 22 * s, 4.5 * s), 2 * s, 2 * s);
    painter.drawRoundedRect(QRectF(180 * s, 140 * s, 18 * s, 4.5 * s), 2 * s, 2 * s);
    painter.drawRoundedRect(QRectF(180 * s, 150 * s, 20 * s, 4.5 * s), 2 * s, 2 * s);
    painter.drawRoundedRect(QRectF(136 * s, 167 * s, 28 * s, 4.5 * s), 2 * s, 2 * s);

    // 4. Digital Stylus
    painter.save();
    painter.translate(195 * s, 170 * s);
    painter.rotate(-38);

    QPainterPath penShadow;
    penShadow.addRoundedRect(QRectF(-14 * s, -4 * s, 28 * s, 122 * s), 12 * s, 12 * s);
    painter.fillPath(penShadow, QColor(0, 0, 0, 90));

    QPainterPath barrel;
    barrel.addRoundedRect(QRectF(-11 * s, -14 * s, 22 * s, 82 * s), 9 * s, 9 * s);
    QLinearGradient barrelGrad(QPointF(-11 * s, 0), QPointF(11 * s, 0));
    barrelGrad.setColorAt(0.0, QColor("#1d4ed8"));
    barrelGrad.setColorAt(0.4, QColor("#60a5fa"));
    barrelGrad.setColorAt(0.7, QColor("#2563eb"));
    barrelGrad.setColorAt(1.0, QColor("#1e40af"));
    painter.fillPath(barrel, barrelGrad);

    // Gloss Reflection
    painter.setPen(QPen(QColor(255, 255, 255, 180), 2.2 * s, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(QPointF(-3 * s, -8 * s), QPointF(-3 * s, 60 * s));

    // Grip Band
    painter.setPen(Qt::NoPen);
    QLinearGradient bandGrad(QPointF(-11 * s, 0), QPointF(11 * s, 0));
    bandGrad.setColorAt(0.0, QColor("#94a3b8"));
    bandGrad.setColorAt(0.5, QColor("#f8fafc"));
    bandGrad.setColorAt(1.0, QColor("#64748b"));
    painter.setBrush(bandGrad);
    painter.drawRoundedRect(QRectF(-11.5 * s, 68 * s, 23 * s, 10 * s), 3 * s, 3 * s);

    // Silver Nib Cone
    QPainterPath nibPath;
    nibPath.moveTo(-10.5 * s, 78 * s);
    nibPath.lineTo(10.5 * s, 78 * s);
    nibPath.lineTo(3.5 * s, 106 * s);
    nibPath.lineTo(-3.5 * s, 106 * s);
    nibPath.closeSubpath();
    QLinearGradient nibGrad(QPointF(-11 * s, 78 * s), QPointF(11 * s, 78 * s));
    nibGrad.setColorAt(0.0, QColor("#64748b"));
    nibGrad.setColorAt(0.45, QColor("#ffffff"));
    nibGrad.setColorAt(1.0, QColor("#475569"));
    painter.fillPath(nibPath, nibGrad);

    // Stylus Tip
    QPainterPath tipPath;
    tipPath.moveTo(-3.5 * s, 106 * s);
    tipPath.lineTo(3.5 * s, 106 * s);
    tipPath.lineTo(0, 116 * s);
    tipPath.closeSubpath();
    painter.fillPath(tipPath, QColor("#1e293b"));

    // Silver Clip
    QPainterPath clipPath;
    clipPath.addRoundedRect(QRectF(11 * s, 12 * s, 4.5 * s, 44 * s), 2 * s, 2 * s);
    painter.fillPath(clipPath, QColor("#e2e8f0"));

    painter.restore();
    painter.end();
    return pixmap;
}

QIcon getAppWindowIcon() {
    QIcon icon;
    const int sizes[] = {16, 20, 24, 32, 48, 64, 128, 256};
    for (int sz : sizes) {
        icon.addPixmap(createMdReaderAppPixmap(sz));
    }
    return icon;
}

QIcon createAppIcon(const QString &name, const QString &colorHex, int size = 22) {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor col(colorHex);
    QPen pen(col);
    pen.setWidthF(1.75);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    const qreal s = size / 22.0;

    if (name == "file_new") {
        QPainterPath p;
        p.moveTo(4.5 * s, 2.5 * s);
        p.lineTo(12.5 * s, 2.5 * s);
        p.lineTo(17.5 * s, 7.5 * s);
        p.lineTo(17.5 * s, 19.5 * s);
        p.lineTo(4.5 * s, 19.5 * s);
        p.closeSubpath();
        painter.drawPath(p);
        painter.drawLine(QPointF(12.5 * s, 2.5 * s), QPointF(12.5 * s, 7.5 * s));
        painter.drawLine(QPointF(12.5 * s, 7.5 * s), QPointF(17.5 * s, 7.5 * s));
        painter.drawLine(QPointF(8 * s, 13.5 * s), QPointF(14 * s, 13.5 * s));
        painter.drawLine(QPointF(11 * s, 10.5 * s), QPointF(11 * s, 16.5 * s));
    } else if (name == "file_open") {
        QPainterPath p;
        p.moveTo(3 * s, 5 * s);
        p.lineTo(8.5 * s, 5 * s);
        p.lineTo(10.5 * s, 7.5 * s);
        p.lineTo(19 * s, 7.5 * s);
        p.lineTo(19 * s, 18 * s);
        p.lineTo(3 * s, 18 * s);
        p.closeSubpath();
        painter.drawPath(p);
        painter.drawLine(QPointF(3 * s, 10 * s), QPointF(19 * s, 10 * s));
    } else if (name == "file_save") {
        QPainterPath p;
        p.moveTo(4 * s, 3 * s);
        p.lineTo(14.5 * s, 3 * s);
        p.lineTo(18 * s, 6.5 * s);
        p.lineTo(18 * s, 19 * s);
        p.lineTo(4 * s, 19 * s);
        p.closeSubpath();
        painter.drawPath(p);
        painter.drawRect(QRectF(7 * s, 3 * s, 6 * s, 5 * s));
        painter.drawRect(QRectF(6.5 * s, 12 * s, 9 * s, 7 * s));
    } else if (name == "file_save_as") {
        QPainterPath p;
        p.moveTo(3.5 * s, 3 * s);
        p.lineTo(13.5 * s, 3 * s);
        p.lineTo(16.5 * s, 6 * s);
        p.lineTo(16.5 * s, 18.5 * s);
        p.lineTo(3.5 * s, 18.5 * s);
        p.closeSubpath();
        painter.drawPath(p);
        painter.drawLine(QPointF(12 * s, 12.5 * s), QPointF(18.5 * s, 12.5 * s));
        painter.drawLine(QPointF(15.2 * s, 9.5 * s), QPointF(15.2 * s, 15.5 * s));
    } else if (name == "bold") {
        QFont font("Segoe UI", qRound(11 * s), QFont::Bold);
        painter.setFont(font);
        painter.drawText(QRectF(0, 0, size, size), Qt::AlignCenter, "B");
    } else if (name == "italic") {
        QFont font("Georgia", qRound(12 * s));
        font.setItalic(true);
        painter.setFont(font);
        painter.drawText(QRectF(0, 0, size, size), Qt::AlignCenter, "I");
    } else if (name == "strikethrough") {
        QFont font("Segoe UI", qRound(11 * s), QFont::DemiBold);
        painter.setFont(font);
        painter.drawText(QRectF(0, 0, size, size), Qt::AlignCenter, "S");
        painter.drawLine(QPointF(3.5 * s, 11 * s), QPointF(18.5 * s, 11 * s));
    } else if (name == "code") {
        painter.drawLine(QPointF(6.5 * s, 7.5 * s), QPointF(3 * s, 11 * s));
        painter.drawLine(QPointF(3 * s, 11 * s), QPointF(6.5 * s, 14.5 * s));
        painter.drawLine(QPointF(15.5 * s, 7.5 * s), QPointF(19 * s, 11 * s));
        painter.drawLine(QPointF(19 * s, 11 * s), QPointF(15.5 * s, 14.5 * s));
        painter.drawLine(QPointF(12.5 * s, 6 * s), QPointF(9.5 * s, 16 * s));
    } else if (name == "quote") {
        QFont font("Georgia", qRound(16 * s), QFont::Bold);
        painter.setFont(font);
        painter.drawText(QRectF(0, -1 * s, size, size), Qt::AlignCenter, "\"");
    } else if (name == "list_bullet") {
        painter.setBrush(col);
        painter.drawEllipse(QRectF(3 * s, 5.5 * s, 2.5 * s, 2.5 * s));
        painter.drawEllipse(QRectF(3 * s, 10.5 * s, 2.5 * s, 2.5 * s));
        painter.drawEllipse(QRectF(3 * s, 15.5 * s, 2.5 * s, 2.5 * s));
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(QPointF(8 * s, 6.7 * s), QPointF(19 * s, 6.7 * s));
        painter.drawLine(QPointF(8 * s, 11.7 * s), QPointF(19 * s, 11.7 * s));
        painter.drawLine(QPointF(8 * s, 16.7 * s), QPointF(19 * s, 16.7 * s));
    } else if (name == "list_task") {
        painter.drawRoundedRect(QRectF(3.5 * s, 3.5 * s, 15 * s, 15 * s), 2.5 * s, 2.5 * s);
        QPainterPath chk;
        chk.moveTo(6.5 * s, 11 * s);
        chk.lineTo(9.5 * s, 14.5 * s);
        chk.lineTo(15.5 * s, 7.5 * s);
        painter.drawPath(chk);
    } else if (name == "table") {
        painter.drawRoundedRect(QRectF(3 * s, 3.5 * s, 16 * s, 15 * s), 2 * s, 2 * s);
        painter.drawLine(QPointF(3 * s, 9 * s), QPointF(19 * s, 9 * s));
        painter.drawLine(QPointF(11 * s, 3.5 * s), QPointF(11 * s, 18.5 * s));
    } else if (name == "link") {
        QPainterPath p1;
        p1.moveTo(11 * s, 7 * s);
        p1.lineTo(14 * s, 7 * s);
        p1.arcTo(QRectF(11.5 * s, 7 * s, 6 * s, 6 * s), 90, -180);
        p1.lineTo(11 * s, 13 * s);
        painter.drawPath(p1);
        QPainterPath p2;
        p2.moveTo(9 * s, 13 * s);
        p2.lineTo(6 * s, 13 * s);
        p2.arcTo(QRectF(2.5 * s, 7 * s, 6 * s, 6 * s), 270, -180);
        p2.lineTo(9 * s, 7 * s);
        painter.drawPath(p2);
        painter.drawLine(QPointF(7 * s, 10 * s), QPointF(13 * s, 10 * s));
    } else if (name == "undo") {
        painter.drawLine(QPointF(5 * s, 9 * s), QPointF(8.5 * s, 5.5 * s));
        painter.drawLine(QPointF(5 * s, 9 * s), QPointF(8.5 * s, 12.5 * s));
        painter.drawLine(QPointF(5 * s, 9 * s), QPointF(12 * s, 9 * s));
        QPainterPath p;
        p.moveTo(12 * s, 9 * s);
        p.quadTo(17.5 * s, 9 * s, 17.5 * s, 14 * s);
        p.quadTo(17.5 * s, 17 * s, 14.5 * s, 17 * s);
        painter.drawPath(p);
    } else if (name == "redo") {
        painter.drawLine(QPointF(17 * s, 9 * s), QPointF(13.5 * s, 5.5 * s));
        painter.drawLine(QPointF(17 * s, 9 * s), QPointF(13.5 * s, 12.5 * s));
        painter.drawLine(QPointF(17 * s, 9 * s), QPointF(10 * s, 9 * s));
        QPainterPath p;
        p.moveTo(10 * s, 9 * s);
        p.quadTo(4.5 * s, 9 * s, 4.5 * s, 14 * s);
        p.quadTo(4.5 * s, 17 * s, 7.5 * s, 17 * s);
        painter.drawPath(p);
    } else if (name == "find") {
        painter.drawEllipse(QRectF(4 * s, 4 * s, 9.5 * s, 9.5 * s));
        painter.drawLine(QPointF(11.5 * s, 11.5 * s), QPointF(18 * s, 18 * s));
    } else if (name == "select_all") {
        painter.drawRoundedRect(QRectF(3.5 * s, 3.5 * s, 15 * s, 15 * s), 2 * s, 2 * s);
        painter.drawLine(QPointF(7 * s, 8 * s), QPointF(15 * s, 8 * s));
        painter.drawLine(QPointF(7 * s, 11 * s), QPointF(15 * s, 11 * s));
        painter.drawLine(QPointF(7 * s, 14 * s), QPointF(13 * s, 14 * s));
    } else if (name == "export_html") {
        painter.drawRoundedRect(QRectF(3.5 * s, 3 * s, 15 * s, 16 * s), 2 * s, 2 * s);
        painter.drawLine(QPointF(7.5 * s, 9 * s), QPointF(6 * s, 11 * s));
        painter.drawLine(QPointF(6 * s, 11 * s), QPointF(7.5 * s, 13 * s));
        painter.drawLine(QPointF(14.5 * s, 9 * s), QPointF(16 * s, 11 * s));
        painter.drawLine(QPointF(16 * s, 11 * s), QPointF(14.5 * s, 13 * s));
        painter.drawLine(QPointF(11.5 * s, 8.5 * s), QPointF(10.5 * s, 13.5 * s));
    } else if (name == "export_pdf") {
        painter.drawRoundedRect(QRectF(4 * s, 3 * s, 14 * s, 16 * s), 2 * s, 2 * s);
        painter.drawLine(QPointF(7 * s, 7.5 * s), QPointF(15 * s, 7.5 * s));
        painter.drawLine(QPointF(7 * s, 11 * s), QPointF(15 * s, 11 * s));
        painter.drawLine(QPointF(7 * s, 14.5 * s), QPointF(12 * s, 14.5 * s));
    } else if (name == "exit") {
        QPainterPath p;
        p.moveTo(11 * s, 4 * s);
        p.lineTo(5 * s, 4 * s);
        p.lineTo(5 * s, 18 * s);
        p.lineTo(11 * s, 18 * s);
        painter.drawPath(p);
        painter.drawLine(QPointF(9 * s, 11 * s), QPointF(18 * s, 11 * s));
        painter.drawLine(QPointF(15 * s, 7.5 * s), QPointF(18.5 * s, 11 * s));
        painter.drawLine(QPointF(15 * s, 14.5 * s), QPointF(18.5 * s, 11 * s));
    } else if (name == "split") {
        painter.drawRoundedRect(QRectF(2.5 * s, 3.5 * s, 17 * s, 15 * s), 2.5 * s, 2.5 * s);
        painter.drawLine(QPointF(11 * s, 3.5 * s), QPointF(11 * s, 18.5 * s));
    } else if (name == "reader") {
        QPainterPath p;
        p.moveTo(11 * s, 18 * s);
        p.lineTo(11 * s, 6 * s);
        p.cubicTo(9 * s, 4.5 * s, 6 * s, 4.5 * s, 3 * s, 6 * s);
        p.lineTo(3 * s, 17.5 * s);
        p.cubicTo(6 * s, 16 * s, 9 * s, 16 * s, 11 * s, 18 * s);
        p.cubicTo(13 * s, 16 * s, 16 * s, 16 * s, 19 * s, 17.5 * s);
        p.lineTo(19 * s, 6 * s);
        p.cubicTo(16 * s, 4.5 * s, 13 * s, 4.5 * s, 11 * s, 6 * s);
        painter.drawPath(p);
    } else if (name == "editor") {
        QPainterPath p;
        p.moveTo(13.5 * s, 3.5 * s);
        p.lineTo(18.5 * s, 8.5 * s);
        p.lineTo(8 * s, 19 * s);
        p.lineTo(3 * s, 19 * s);
        p.lineTo(3 * s, 14 * s);
        p.closeSubpath();
        painter.drawPath(p);
        painter.drawLine(QPointF(11.5 * s, 5.5 * s), QPointF(16.5 * s, 10.5 * s));
    } else if (name == "sync_on") {
        painter.drawArc(QRectF(3.5 * s, 3.5 * s, 15 * s, 15 * s), 45 * 16, 210 * 16);
        painter.drawLine(QPointF(15 * s, 3 * s), QPointF(18.5 * s, 5.5 * s));
        painter.drawLine(QPointF(15.5 * s, 9 * s), QPointF(18.5 * s, 5.5 * s));
        painter.drawLine(QPointF(7 * s, 19 * s), QPointF(3.5 * s, 16.5 * s));
        painter.drawLine(QPointF(6.5 * s, 13 * s), QPointF(3.5 * s, 16.5 * s));
    } else if (name == "sync_off") {
        painter.drawArc(QRectF(3.5 * s, 3.5 * s, 15 * s, 15 * s), 45 * 16, 210 * 16);
        painter.drawLine(QPointF(15 * s, 3 * s), QPointF(18.5 * s, 5.5 * s));
        painter.drawLine(QPointF(15.5 * s, 9 * s), QPointF(18.5 * s, 5.5 * s));
        painter.drawLine(QPointF(7 * s, 19 * s), QPointF(3.5 * s, 16.5 * s));
        painter.drawLine(QPointF(6.5 * s, 13 * s), QPointF(3.5 * s, 16.5 * s));
        QPen slash(col);
        slash.setWidthF(2.0);
        painter.setPen(slash);
        painter.drawLine(QPointF(2.5 * s, 2.5 * s), QPointF(19.5 * s, 19.5 * s));
    } else if (name == "theme_dark") {
        QPainterPath p;
        p.moveTo(12.5 * s, 3 * s);
        p.arcTo(QRectF(3 * s, 3 * s, 16 * s, 16 * s), 90, 270);
        p.arcTo(QRectF(7.5 * s, 3 * s, 11.5 * s, 16 * s), 270, -180);
        p.closeSubpath();
        painter.drawPath(p);
    } else if (name == "theme_light") {
        painter.drawEllipse(QRectF(6.5 * s, 6.5 * s, 9 * s, 9 * s));
        const qreal rays[8][4] = {
            {11, 2, 11, 4.5}, {11, 17.5, 11, 20},
            {2, 11, 4.5, 11}, {17.5, 11, 20, 11},
            {4.5, 4.5, 6.5, 6.5}, {15.5, 15.5, 17.5, 17.5},
            {15.5, 4.5, 13.5, 6.5}, {4.5, 17.5, 6.5, 15.5}
        };
        for (const auto &r : rays) {
            painter.drawLine(QPointF(r[0] * s, r[1] * s), QPointF(r[2] * s, r[3] * s));
        }
    } else if (name == "sidebar") {
        painter.drawRoundedRect(QRectF(2.5 * s, 3.5 * s, 17 * s, 15 * s), 2.5 * s, 2.5 * s);
        painter.drawLine(QPointF(8.5 * s, 3.5 * s), QPointF(8.5 * s, 18.5 * s));
        painter.drawLine(QPointF(4 * s, 8 * s), QPointF(7 * s, 8 * s));
        painter.drawLine(QPointF(4 * s, 12 * s), QPointF(7 * s, 12 * s));
    } else if (name == "zoom_in") {
        painter.drawEllipse(QRectF(3 * s, 3 * s, 11 * s, 11 * s));
        painter.drawLine(QPointF(11.5 * s, 11.5 * s), QPointF(18 * s, 18 * s));
        painter.drawLine(QPointF(6 * s, 8.5 * s), QPointF(11 * s, 8.5 * s));
        painter.drawLine(QPointF(8.5 * s, 6 * s), QPointF(8.5 * s, 11 * s));
    } else if (name == "zoom_out") {
        painter.drawEllipse(QRectF(3 * s, 3 * s, 11 * s, 11 * s));
        painter.drawLine(QPointF(11.5 * s, 11.5 * s), QPointF(18 * s, 18 * s));
        painter.drawLine(QPointF(6 * s, 8.5 * s), QPointF(11 * s, 8.5 * s));
    } else if (name == "zoom_reset") {
        QFont font("Segoe UI", qRound(7.5 * s), QFont::Bold);
        painter.setFont(font);
        painter.drawText(QRectF(0, 0, size, size), Qt::AlignCenter, "1:1");
    } else if (name == "chevron_up") {
        painter.drawLine(QPointF(5 * s, 13 * s), QPointF(11 * s, 7 * s));
        painter.drawLine(QPointF(11 * s, 7 * s), QPointF(17 * s, 13 * s));
    } else if (name == "chevron_down") {
        painter.drawLine(QPointF(5 * s, 8 * s), QPointF(11 * s, 14 * s));
        painter.drawLine(QPointF(11 * s, 14 * s), QPointF(17 * s, 8 * s));
    } else if (name == "close") {
        painter.drawLine(QPointF(5.5 * s, 5.5 * s), QPointF(16.5 * s, 16.5 * s));
        painter.drawLine(QPointF(16.5 * s, 5.5 * s), QPointF(5.5 * s, 16.5 * s));
    } else if (name == "settings") {
        painter.drawEllipse(QRectF(7 * s, 7 * s, 8 * s, 8 * s));
        QPen cogPen(col);
        cogPen.setWidthF(2.0);
        painter.setPen(cogPen);
        for (int angle = 0; angle < 360; angle += 45) {
            qreal rad = angle * M_PI / 180.0;
            qreal cx = 11 * s, cy = 11 * s;
            painter.drawLine(
                QPointF(cx + 5.8 * s * std::cos(rad), cy + 5.8 * s * std::sin(rad)),
                QPointF(cx + 9.2 * s * std::cos(rad), cy + 9.2 * s * std::sin(rad))
            );
        }
        painter.setPen(pen);
        painter.drawEllipse(QRectF(9.5 * s, 9.5 * s, 3 * s, 3 * s));
    }

    painter.end();
    return QIcon(pixmap);
}

QString generateQss(const QString &theme = "dark") {
    const ThemePalette &p = getPalette(theme);
    return QString(R"(
    QMainWindow, QWidget {
        background-color: %1;
        color: %2;
        font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif;
        font-size: 13px;
    }
    QMenuBar {
        background-color: %3;
        color: %2;
        border-bottom: 1px solid %4;
        padding: 4px 6px;
        font-size: 13px;
    }
    QMenuBar::item {
        background-color: transparent;
        color: %2;
        padding: 6px 14px;
        border-radius: 5px;
        margin: 1px 3px;
        font-weight: 500;
    }
    QMenuBar::item:selected {
        background-color: %5;
        color: %6;
        font-weight: 700;
    }
    QMenuBar::item:pressed {
        background-color: %7;
        color: %6;
        font-weight: 700;
    }
    QMenu {
        background-color: %3;
        color: %2;
        border: 1px solid %4;
        border-radius: 8px;
        padding: 6px;
    }
    QMenu::item {
        background-color: transparent;
        color: %2;
        padding: 8px 32px 8px 14px;
        border-radius: 5px;
        margin: 2px 2px;
        font-size: 13px;
        font-weight: 500;
        border: 1px solid transparent;
    }
    QMenu::item:selected {
        background-color: %5;
        color: %6;
        border: 1px solid %5;
        font-weight: 600;
    }
    QMenu::item:pressed {
        background-color: %7;
        color: %6;
    }
    QMenu::item:disabled {
        color: %8;
        background-color: transparent;
        border-color: transparent;
    }
    QMenu::separator {
        height: 1px;
        background-color: %4;
        margin: 5px 8px;
    }
    #sidebarTitle {
        color: %8;
        font-size: 11px;
        font-weight: 700;
        letter-spacing: 0.8px;
        padding: 6px 8px;
    }
    QToolBar {
        background-color: %3;
        border-bottom: 1px solid %4;
        spacing: 4px;
        padding: 5px 8px;
    }
    QToolButton {
        background-color: transparent;
        color: %2;
        border: 1px solid transparent;
        border-radius: 5px;
        padding: 5px 7px;
        font-weight: 500;
        margin: 1px;
    }
    QToolButton:hover {
        background-color: %9;
        border-color: %4;
    }
    QToolButton:pressed {
        background-color: %10;
    }
    QToolButton:checked {
        background-color: %11;
        border-color: %12;
        color: %12;
    }
    QSplitter::handle {
        background-color: %4;
    }
    QSplitter::handle:hover {
        background-color: %12;
    }
    QSplitter::handle:horizontal {
        width: 2px;
    }
    QSplitter::handle:vertical {
        height: 2px;
    }
    QTreeWidget {
        background-color: %3;
        border: none;
        color: %2;
        outline: none;
        padding: 4px;
    }
    QTreeWidget::item {
        padding: 5px 8px;
        border-radius: 4px;
    }
    QTreeWidget::item:hover {
        background-color: %9;
    }
    QTreeWidget::item:selected {
        background-color: %11;
        color: %12;
        font-weight: 600;
    }
    QStatusBar {
        background-color: %3;
        color: %13;
        border-top: 1px solid %4;
        font-size: 12px;
        padding: 2px 8px;
    }
    QLineEdit {
        background-color: %14;
        color: %2;
        border: 1px solid %4;
        border-radius: 5px;
        padding: 5px 9px;
        selection-background-color: %12;
    }
    QLineEdit:focus {
        border: 1px solid %12;
    }
    QPushButton {
        background-color: %14;
        color: %2;
        border: 1px solid %4;
        border-radius: 5px;
        padding: 5px 12px;
        font-weight: 500;
    }
    QPushButton:hover {
        background-color: %9;
        border-color: %12;
    }
    QPushButton:pressed {
        background-color: %10;
    }
    QCheckBox {
        spacing: 6px;
        color: %13;
    }
    QCheckBox::indicator {
        width: 15px;
        height: 15px;
        border: 1px solid %4;
        border-radius: 3px;
        background-color: %14;
    }
    QCheckBox::indicator:checked {
        background-color: %12;
        border-color: %12;
    }
    QScrollBar:vertical {
        background: %3;
        width: 10px;
        margin: 0px;
    }
    QScrollBar::handle:vertical {
        background: %9;
        min-height: 24px;
        border-radius: 5px;
    }
    QScrollBar::handle:vertical:hover {
        background: %8;
    }
    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
        height: 0px;
    }
    QTabWidget::pane {
        border: 1px solid %4;
        border-radius: 6px;
        background-color: %3;
        top: -1px;
    }
    QTabBar::tab {
        background-color: %14;
        color: %13;
        border: 1px solid %4;
        border-bottom: none;
        border-top-left-radius: 6px;
        border-top-right-radius: 6px;
        padding: 8px 18px;
        margin-right: 3px;
        font-weight: 500;
    }
    QTabBar::tab:selected {
        background-color: %3;
        color: %12;
        border-color: %4;
        border-bottom: 2px solid %12;
        font-weight: 600;
    }
    QTabBar::tab:hover:!selected {
        background-color: %9;
        color: %2;
    }
    QGroupBox {
        border: 1px solid %4;
        border-radius: 6px;
        margin-top: 14px;
        padding: 14px 10px 10px 10px;
        font-weight: 600;
        color: %2;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        subcontrol-position: top left;
        left: 12px;
        padding: 0 4px;
        color: %12;
    }
    QComboBox, QFontComboBox, QSpinBox {
        background-color: %14;
        color: %2;
        border: 1px solid %4;
        border-radius: 5px;
        padding: 5px 8px;
        min-height: 22px;
    }
    QComboBox:focus, QFontComboBox:focus, QSpinBox:focus {
        border-color: %12;
    }
    QComboBox QAbstractItemView, QFontComboBox QAbstractItemView {
        background-color: %3;
        color: %2;
        border: 1px solid %4;
        selection-background-color: %12;
        selection-color: #ffffff;
    }
    )")
    .arg(p.bg_main)
    .arg(p.text_primary)
    .arg(p.bg_secondary)
    .arg(p.border)
    .arg(p.menu_highlight)
    .arg(p.menu_text_highlight)
    .arg(p.accent_hover)
    .arg(p.text_dim)
    .arg(p.bg_hover)
    .arg(p.bg_selected)
    .arg(p.accent_bg)
    .arg(p.accent)
    .arg(p.text_secondary)
    .arg(p.bg_tertiary);
}

QString escapeHtml(const QString &text) {
    QString res = text;
    res.replace("&", "&amp;");
    res.replace("<", "&lt;");
    res.replace(">", "&gt;");
    res.replace("\"", "&quot;");
    return res;
}

QString renderBasicMarkdown(const QString &text) {
    QStringList lines = text.split("\n");
    QStringList htmlLines;
    bool inCodeBlock = false;
    bool inList = false;

    QRegularExpression hRegex("^(#{1,6})\\s+(.*)$");
    QRegularExpression hrRegex("^(\\*\\*\\*+|---|___)$");
    QRegularExpression listRegex("^\\s*[-*+]\\s+(.*)$");

    for (const QString &line : lines) {
        if (line.trimmed().startsWith("```")) {
            if (inCodeBlock) {
                htmlLines.append("</code></pre>");
                inCodeBlock = false;
            } else {
                if (inList) { htmlLines.append("</ul>"); inList = false; }
                QString lang = line.trimmed().mid(3).trimmed();
                htmlLines.append(QString("<pre><code class=\"lang-%1\">").arg(lang));
                inCodeBlock = true;
            }
            continue;
        }

        if (inCodeBlock) {
            htmlLines.append(escapeHtml(line));
            continue;
        }

        auto hMatch = hRegex.match(line);
        if (hMatch.hasMatch()) {
            if (inList) { htmlLines.append("</ul>"); inList = false; }
            int level = hMatch.captured(1).length();
            QString content = escapeHtml(hMatch.captured(2));
            htmlLines.append(QString("<h%1>%2</h%1>").arg(level).arg(content));
            continue;
        }

        if (hrRegex.match(line.trimmed()).hasMatch()) {
            if (inList) { htmlLines.append("</ul>"); inList = false; }
            htmlLines.append("<hr/>");
            continue;
        }

        if (line.startsWith("> ")) {
            if (inList) { htmlLines.append("</ul>"); inList = false; }
            htmlLines.append(QString("<blockquote><p>%1</p></blockquote>").arg(escapeHtml(line.mid(2))));
            continue;
        }

        auto listMatch = listRegex.match(line);
        if (listMatch.hasMatch()) {
            if (!inList) { htmlLines.append("<ul>"); inList = true; }
            QString item = listMatch.captured(1);
            if (item.startsWith("[ ] ")) {
                item = "<input type=\"checkbox\" disabled> " + escapeHtml(item.mid(4));
            } else if (item.startsWith("[x] ", Qt::CaseInsensitive)) {
                item = "<input type=\"checkbox\" checked disabled> " + escapeHtml(item.mid(4));
            } else {
                item = escapeHtml(item);
            }
            // Inline markup
            item.replace(QRegularExpression("\\*\\*(.*?)\\*\\*"), "<strong>\\1</strong>");
            item.replace(QRegularExpression("\\*(.*?)\\*"), "<em>\\1</em>");
            htmlLines.append(QString("<li>%1</li>").arg(item));
            continue;
        }

        if (inList) {
            htmlLines.append("</ul>");
            inList = false;
        }

        if (line.trimmed().isEmpty()) {
            htmlLines.append("<br/>");
        } else {
            QString escaped = escapeHtml(line);
            escaped.replace(QRegularExpression("\\*\\*(.*?)\\*\\*"), "<strong>\\1</strong>");
            escaped.replace(QRegularExpression("\\*(.*?)\\*"), "<em>\\1</em>");
            escaped.replace(QRegularExpression("~~(.*?)~~"), "<del>\\1</del>");
            escaped.replace(QRegularExpression("`([^`]+)`"), "<code>\\1</code>");
            escaped.replace(QRegularExpression("\\[(.*?)\\]\\((.*?)\\)"), "<a href=\"\\2\">\\1</a>");
            htmlLines.append(QString("<p>%1</p>").arg(escaped));
        }
    }

    if (inCodeBlock) htmlLines.append("</code></pre>");
    if (inList) htmlLines.append("</ul>");

    return htmlLines.join("\n");
}

QString convertMarkdownToHtml(const QString &mdText, const QString &theme = "dark",
                              const QString &readerFont = "Segoe UI", int baseFontSize = 15) {
    const ThemePalette &p = getPalette(theme);
    QString body = renderBasicMarkdown(mdText);

    QString fontCss = (readerFont != "System Default")
        ? QString("'%1', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif").arg(readerFont)
        : "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif";

    return QString(R"(<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<style>
    body {
        font-family: %1;
        font-size: %2px;
        line-height: 1.7;
        color: %3;
        background-color: %4;
        padding: 28px 40px;
        margin: 0;
        word-wrap: break-word;
    }
    h1, h2, h3, h4, h5, h6 {
        margin-top: 24px;
        margin-bottom: 14px;
        font-weight: 600;
        line-height: 1.3;
        color: %3;
    }
    h1 {
        font-size: 2.1em;
        padding-bottom: 0.35em;
        border-bottom: 1px solid %5;
        color: %6;
    }
    h2 {
        font-size: 1.55em;
        padding-bottom: 0.3em;
        border-bottom: 1px solid %5;
    }
    h3 { font-size: 1.25em; }
    h4 { font-size: 1.05em; }
    p, blockquote, ul, ol, dl, table, pre {
        margin-top: 0;
        margin-bottom: 16px;
    }
    a {
        color: %6;
        text-decoration: none;
        font-weight: 500;
    }
    a:hover {
        text-decoration: underline;
    }
    code {
        padding: 0.2em 0.45em;
        margin: 0;
        font-size: 87%;
        background-color: %7;
        border-radius: 4px;
        border: 1px solid %5;
        font-family: 'Cascadia Code', 'Fira Code', 'Consolas', monospace;
        color: %8;
    }
    pre {
        padding: 16px;
        overflow: auto;
        font-size: 90%;
        line-height: 1.5;
        background-color: %7;
        border: 1px solid %5;
        border-radius: 7px;
    }
    pre code {
        background: transparent;
        padding: 0;
        border: 0;
        color: %3;
    }
    blockquote {
        padding: 4px 16px;
        color: %9;
        border-left: 4px solid %6;
        margin: 0 0 16px 0;
        background-color: %10;
        border-radius: 0 6px 6px 0;
    }
    table {
        border-spacing: 0;
        border-collapse: collapse;
        width: 100%;
        margin-bottom: 18px;
    }
    table th, table td {
        padding: 9px 14px;
        border: 1px solid %5;
    }
    table th {
        font-weight: 600;
        background-color: %10;
        color: %3;
    }
    table tr:nth-child(2n) {
        background-color: %10;
    }
    hr {
        height: 1px;
        padding: 0;
        margin: 28px 0;
        background-color: %5;
        border: 0;
    }
    input[type="checkbox"] {
        margin-right: 8px;
    }
    ul, ol {
        padding-left: 24px;
    }
    li {
        margin-bottom: 4px;
    }
</style>
</head>
<body>
%11
</body>
</html>)")
    .arg(fontCss)
    .arg(baseFontSize)
    .arg(p.text_primary)
    .arg(p.preview_bg)
    .arg(p.preview_border)
    .arg(p.accent)
    .arg(p.preview_code_bg)
    .arg(p.warning)
    .arg(p.text_secondary)
    .arg(p.bg_secondary)
    .arg(body);
}

class MarkdownSyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    struct Rule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    explicit MarkdownSyntaxHighlighter(QTextDocument *parent = nullptr, const QString &theme = "dark")
        : QSyntaxHighlighter(parent), m_theme(theme) {
        setupRules();
    }

    void setTheme(const QString &theme) {
        m_theme = theme;
        setupRules();
        rehighlight();
    }

protected:
    void highlightBlock(const QString &text) override {
        for (const auto &rule : m_rules) {
            auto it = rule.pattern.globalMatch(text);
            while (it.hasNext()) {
                auto match = it.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
    }

private:
    void setupRules() {
        m_rules.clear();
        const ThemePalette &p = getPalette(m_theme);

        // Headings
        QTextCharFormat h1; h1.setForeground(QColor(p.accent)); h1.setFontWeight(QFont::Bold);
        m_rules.append({QRegularExpression("^#\\s+.*$"), h1});

        QTextCharFormat h2; h2.setForeground(QColor(p.success)); h2.setFontWeight(QFont::Bold);
        m_rules.append({QRegularExpression("^##\\s+.*$"), h2});

        QTextCharFormat h3; h3.setForeground(QColor(p.warning)); h3.setFontWeight(QFont::Bold);
        m_rules.append({QRegularExpression("^###\\s+.*$"), h3});

        QTextCharFormat hRest; hRest.setForeground(QColor(p.text_secondary)); hRest.setFontWeight(QFont::Bold);
        m_rules.append({QRegularExpression("^#{4,6}\\s+.*$"), hRest});

        // Bold & Italic
        QTextCharFormat bold; bold.setFontWeight(QFont::Bold); bold.setForeground(QColor(p.accent_hover));
        m_rules.append({QRegularExpression("\\*\\*([^*]+)\\*\\*"), bold});

        QTextCharFormat italic; italic.setFontItalic(true); italic.setForeground(QColor(p.text_secondary));
        m_rules.append({QRegularExpression("(?<!\\*)\\*([^*]+)\\*(?!\\*)"), italic});

        QTextCharFormat strike; strike.setFontStrikeOut(true); strike.setForeground(QColor(p.text_dim));
        m_rules.append({QRegularExpression("~~([^~]+)~~"), strike});

        QTextCharFormat code; code.setForeground(QColor(p.warning));
        code.setFontFamilies({"Cascadia Code", "Consolas", "Courier New"});
        m_rules.append({QRegularExpression("`([^`]+)`"), code});

        QTextCharFormat quote; quote.setForeground(QColor(p.text_dim)); quote.setFontItalic(true);
        m_rules.append({QRegularExpression("^>\\s.*$"), quote});

        QTextCharFormat link; link.setForeground(QColor(p.accent)); link.setFontUnderline(true);
        m_rules.append({QRegularExpression("\\[(.*?)\\]\\((.*?)\\)"), link});

        QTextCharFormat task; task.setForeground(QColor(p.success)); task.setFontWeight(QFont::Bold);
        m_rules.append({QRegularExpression("^\\s*-\\s+\\[[ xX]\\]"), task});
    }

    QString m_theme;
    QVector<Rule> m_rules;
};

class CodeEditor;

class LineNumberArea : public QWidget {
public:
    explicit LineNumberArea(CodeEditor *editor);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *m_editor;
};

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    explicit CodeEditor(const QString &theme = "dark", QWidget *parent = nullptr)
        : QPlainTextEdit(parent), m_theme(theme), m_showLineNumbers(true), m_enableLineHighlight(true) {
        m_lineNumberArea = new LineNumberArea(this);

        QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setPointSize(11);
        setFont(font);
        setTabStopDistance(fontMetrics().horizontalAdvance(' ') * 4);
        setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

        connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
        connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
        connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

        m_highlighter = new MarkdownSyntaxHighlighter(document(), m_theme);
        updateLineNumberAreaWidth(0);
        highlightCurrentLine();
    }

    void setTheme(const QString &theme) {
        m_theme = theme;
        m_highlighter->setTheme(theme);
        highlightCurrentLine();
        m_lineNumberArea->update();
    }

    void setEditorFont(const QString &family, int size) {
        QFont f = font();
        if (!family.isEmpty()) f.setFamily(family);
        if (size > 0) f.setPointSize(size);
        setFont(f);
        updateLineNumberAreaWidth(0);
    }

    void setTabSpaces(int spaces) {
        setTabStopDistance(fontMetrics().horizontalAdvance(' ') * qMax(1, spaces));
    }

    void setLineNumbersVisible(bool visible) {
        m_showLineNumbers = visible;
        m_lineNumberArea->setVisible(visible);
        updateLineNumberAreaWidth(0);
    }

    void setLineHighlighting(bool enabled) {
        m_enableLineHighlight = enabled;
        highlightCurrentLine();
    }

    int lineNumberAreaWidth() {
        if (!m_showLineNumbers) return 0;
        int digits = 1;
        int maxNum = qMax(1, blockCount());
        while (maxNum >= 10) {
            maxNum /= 10;
            ++digits;
        }
        return 16 + fontMetrics().horizontalAdvance('9') * digits;
    }

    void lineNumberAreaPaintEvent(QPaintEvent *event) {
        if (!m_showLineNumbers) return;
        const ThemePalette &p = getPalette(m_theme);
        QPainter painter(m_lineNumberArea);
        painter.fillRect(event->rect(), QColor(p.editor_gutter));

        QTextBlock block = firstVisibleBlock();
        int blockNumber = block.blockNumber();
        int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
        int bottom = top + qRound(blockBoundingRect(block).height());

        painter.setPen(QColor(p.text_dim));
        painter.setFont(font());

        while (block.isValid() && top <= event->rect().bottom()) {
            if (block.isVisible() && bottom >= event->rect().top()) {
                QString number = QString::number(blockNumber + 1);
                painter.drawText(0, top, m_lineNumberArea->width() - 8, fontMetrics().height(),
                                 Qt::AlignRight, number);
            }
            block = block.next();
            top = bottom;
            bottom = top + qRound(blockBoundingRect(block).height());
            ++blockNumber;
        }
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        QPlainTextEdit::resizeEvent(event);
        QRect cr = contentsRect();
        m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
    }

private slots:
    void updateLineNumberAreaWidth(int) {
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    }

    void updateLineNumberArea(const QRect &rect, int dy) {
        if (!m_showLineNumbers) return;
        if (dy) {
            m_lineNumberArea->scroll(0, dy);
        } else {
            m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
        }
        if (rect.contains(viewport()->rect())) {
            updateLineNumberAreaWidth(0);
        }
    }

    void highlightCurrentLine() {
        QList<QTextEdit::ExtraSelection> extraSelections;
        if (!isReadOnly() && m_enableLineHighlight) {
            const ThemePalette &p = getPalette(m_theme);
            QTextEdit::ExtraSelection selection;
            selection.format.setBackground(QColor(p.editor_current_line));
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = textCursor();
            selection.cursor.clearSelection();
            extraSelections.append(selection);
        }
        setExtraSelections(extraSelections);
    }

private:
    QString m_theme;
    QWidget *m_lineNumberArea;
    MarkdownSyntaxHighlighter *m_highlighter;
    bool m_showLineNumbers;
    bool m_enableLineHighlight;
};

LineNumberArea::LineNumberArea(CodeEditor *editor) : QWidget(editor), m_editor(editor) {}
QSize LineNumberArea::sizeHint() const { return QSize(m_editor->lineNumberAreaWidth(), 0); }
void LineNumberArea::paintEvent(QPaintEvent *event) { m_editor->lineNumberAreaPaintEvent(event); }

class FindReplaceBar : public QFrame {
    Q_OBJECT
public:
    explicit FindReplaceBar(const QString &theme = "dark", QWidget *parent = nullptr)
        : QFrame(parent), m_theme(theme) {
        setFrameShape(QFrame::NoFrame);
        setVisible(false);
        initUi();
    }

    void setTheme(const QString &theme) {
        m_theme = theme;
        QString fg = getPalette(theme).text_primary;
        m_btnFindPrev->setIcon(createAppIcon("chevron_up", fg, 16));
        m_btnFindNext->setIcon(createAppIcon("chevron_down", fg, 16));
        m_btnClose->setIcon(createAppIcon("close", fg, 14));
    }

    QLineEdit* findInput() { return m_findInput; }
    bool isMatchCase() const { return m_matchCaseCheck->isChecked(); }

signals:
    void findNextSignal(const QString &query, bool matchCase);
    void findPrevSignal(const QString &query, bool matchCase);
    void replaceSignal(const QString &find, const QString &replace);
    void replaceAllSignal(const QString &find, const QString &replace);

private:
    void initUi() {
        auto *layout = new QHBoxLayout(this);
        layout->setContentsMargins(10, 6, 10, 6);
        layout->setSpacing(6);

        QString fg = getPalette(m_theme).text_primary;

        m_findInput = new QLineEdit(this);
        m_findInput->setPlaceholderText("Find...");
        layout->addWidget(m_findInput, 2);

        m_replaceInput = new QLineEdit(this);
        m_replaceInput->setPlaceholderText("Replace with...");
        layout->addWidget(m_replaceInput, 2);

        m_matchCaseCheck = new QCheckBox("Aa", this);
        m_matchCaseCheck->setToolTip("Match Case");
        layout->addWidget(m_matchCaseCheck);

        m_btnFindPrev = new QPushButton(this);
        m_btnFindPrev->setIcon(createAppIcon("chevron_up", fg, 16));
        m_btnFindPrev->setToolTip("Previous match (Shift+F3)");
        layout->addWidget(m_btnFindPrev);

        m_btnFindNext = new QPushButton(this);
        m_btnFindNext->setIcon(createAppIcon("chevron_down", fg, 16));
        m_btnFindNext->setToolTip("Next match (F3 / Enter)");
        layout->addWidget(m_btnFindNext);

        m_btnReplace = new QPushButton("Replace", this);
        layout->addWidget(m_btnReplace);

        m_btnReplaceAll = new QPushButton("All", this);
        layout->addWidget(m_btnReplaceAll);

        m_btnClose = new QPushButton(this);
        m_btnClose->setIcon(createAppIcon("close", fg, 14));
        layout->addWidget(m_btnClose);

        connect(m_findInput, &QLineEdit::textChanged, this, [this]() {
            if (!m_findInput->text().isEmpty()) emit findNextSignal(m_findInput->text(), isMatchCase());
        });
        connect(m_findInput, &QLineEdit::returnPressed, this, [this]() {
            if (!m_findInput->text().isEmpty()) emit findNextSignal(m_findInput->text(), isMatchCase());
        });
        connect(m_btnFindNext, &QPushButton::clicked, this, [this]() {
            if (!m_findInput->text().isEmpty()) emit findNextSignal(m_findInput->text(), isMatchCase());
        });
        connect(m_btnFindPrev, &QPushButton::clicked, this, [this]() {
            if (!m_findInput->text().isEmpty()) emit findPrevSignal(m_findInput->text(), isMatchCase());
        });
        connect(m_btnReplace, &QPushButton::clicked, this, [this]() {
            emit replaceSignal(m_findInput->text(), m_replaceInput->text());
        });
        connect(m_btnReplaceAll, &QPushButton::clicked, this, [this]() {
            emit replaceAllSignal(m_findInput->text(), m_replaceInput->text());
        });
        connect(m_btnClose, &QPushButton::clicked, this, &FindReplaceBar::hide);
    }

    QString m_theme;
    QLineEdit *m_findInput;
    QLineEdit *m_replaceInput;
    QCheckBox *m_matchCaseCheck;
    QPushButton *m_btnFindPrev;
    QPushButton *m_btnFindNext;
    QPushButton *m_btnReplace;
    QPushButton *m_btnReplaceAll;
    QPushButton *m_btnClose;
};

class MarkdownPreviewWidget : public QWidget {
    Q_OBJECT
public:
    explicit MarkdownPreviewWidget(const QString &theme = "dark", QWidget *parent = nullptr)
        : QWidget(parent), m_theme(theme), m_readerFont("Segoe UI"), m_baseFontSize(15) {
        auto *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);

        m_browser = new QTextBrowser(this);
        m_browser->setOpenExternalLinks(true);
        layout->addWidget(m_browser);

        connect(m_browser->verticalScrollBar(), &QScrollBar::valueChanged, this, [this](int val) {
            int maxV = m_browser->verticalScrollBar()->maximum();
            qreal ratio = (maxV > 0) ? static_cast<qreal>(val) / maxV : 0.0;
            emit scrolledByUser(ratio);
        });
    }

    void setContent(const QString &mdText) {
        m_lastMd = mdText;
        QString fullHtml = convertMarkdownToHtml(mdText, m_theme, m_readerFont, m_baseFontSize);
        m_browser->setHtml(fullHtml);
    }

    void setPreviewTypography(const QString &fontName, int size, const QString &currentMd) {
        m_readerFont = fontName;
        m_baseFontSize = size;
        setContent(currentMd);
    }

    void setTheme(const QString &theme, const QString &currentMd) {
        m_theme = theme;
        setContent(currentMd);
    }

    void scrollToRatio(qreal ratio) {
        ratio = qBound(0.0, ratio, 1.0);
        auto *sb = m_browser->verticalScrollBar();
        sb->setValue(qRound(ratio * sb->maximum()));
    }

    void zoomInPreview() { m_browser->zoomIn(1); }
    void zoomOutPreview() { m_browser->zoomOut(1); }
    void zoomResetPreview() {
        QFont f = m_browser->font();
        f.setPointSize(11);
        m_browser->setFont(f);
    }

signals:
    void scrolledByUser(qreal ratio);

private:
    QString m_theme;
    QString m_readerFont;
    int m_baseFontSize;
    QString m_lastMd;
    QTextBrowser *m_browser;
};

class OutlineWidget : public QTreeWidget {
    Q_OBJECT
public:
    explicit OutlineWidget(QWidget *parent = nullptr) : QTreeWidget(parent) {
        setHeaderHidden(true);
        connect(this, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem *item, int) {
            int line = item->data(0, Qt::UserRole).toInt();
            emit headingClicked(line);
        });
    }

    void updateOutline(const QString &text) {
        clear();
        QStringList lines = text.split("\n");
        QMap<int, QTreeWidgetItem*> rootItems;
        rootItems[0] = invisibleRootItem();

        QRegularExpression hRegex("^(#{1,6})\\s+(.*)$");
        for (int i = 0; i < lines.size(); ++i) {
            auto match = hRegex.match(lines[i]);
            if (match.hasMatch()) {
                int level = match.captured(1).length();
                QString title = match.captured(2).trimmed();

                auto *item = new QTreeWidgetItem(QStringList() << title);
                item->setData(0, Qt::UserRole, i);

                int parentLevel = level - 1;
                while (parentLevel > 0 && !rootItems.contains(parentLevel)) {
                    --parentLevel;
                }

                QTreeWidgetItem *parentItem = rootItems.value(parentLevel, invisibleRootItem());
                parentItem->addChild(item);
                rootItems[level] = item;
                item->setExpanded(true);
            }
        }
    }

signals:
    void headingClicked(int lineNumber);
};

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(const QVariantMap &cfg, QWidget *parent = nullptr)
        : QDialog(parent), m_cfg(cfg) {
        setWindowTitle("MD Reader - Settings");
        setWindowIcon(getAppWindowIcon());
        resize(540, 480);
        setModal(true);
        initUi();
    }

    QVariantMap getValues() const {
        QVariantMap v;
        int tIdx = m_comboTheme->currentIndex();
        v["theme_mode"] = (tIdx == 0) ? "system" : ((tIdx == 1) ? "dark" : "light");

        int vIdx = m_comboView->currentIndex();
        v["default_view_mode"] = (vIdx == 0) ? "split" : ((vIdx == 1) ? "preview" : "editor");

        v["show_outline_on_start"] = m_chkShowOutline->isChecked();
        v["sync_scroll_default"] = m_chkSyncDefault->isChecked();
        v["editor_font_family"] = m_comboEdFont->currentFont().family();
        v["editor_font_size"] = m_spinEdFontSize->value();
        v["tab_size"] = m_spinTabSize->value();
        v["show_line_numbers"] = m_chkLineNumbers->isChecked();
        v["highlight_current_line"] = m_chkHighlightLine->isChecked();
        v["editor_word_wrap"] = m_chkWordWrap->isChecked();
        v["reader_font_family"] = m_comboRdFont->currentText();
        v["reader_font_size"] = m_spinRdFontSize->value();
        v["render_debounce_ms"] = m_spinDebounce->value();
        return v;
    }

signals:
    void settingsApplied(const QVariantMap &cfg);

private:
    void initUi() {
        auto *mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(12);
        mainLayout->setContentsMargins(16, 16, 16, 16);

        auto *tabs = new QTabWidget(this);

        // General
        auto *tabGen = new QWidget(this);
        auto *lGen = new QVBoxLayout(tabGen);
        auto *grpTheme = new QGroupBox("Theme & Mode", tabGen);
        auto *fTheme = new QFormLayout(grpTheme);

        m_comboTheme = new QComboBox(grpTheme);
        m_comboTheme->addItems({"System (Auto Detect)", "Dark Mode", "Light Mode"});
        QString mode = m_cfg.value("theme_mode", "system").toString();
        m_comboTheme->setCurrentIndex((mode == "dark") ? 1 : ((mode == "light") ? 2 : 0));
        fTheme->addRow("Appearance:", m_comboTheme);

        m_comboView = new QComboBox(grpTheme);
        m_comboView->addItems({"Side-by-Side (Split)", "Reader Only", "Editor Only"});
        QString dView = m_cfg.value("default_view_mode", "split").toString();
        m_comboView->setCurrentIndex((dView == "preview") ? 1 : ((dView == "editor") ? 2 : 0));
        fTheme->addRow("Default View Layout:", m_comboView);
        lGen->addWidget(grpTheme);

        auto *grpUi = new QGroupBox("Startup & Panels", tabGen);
        auto *lUi = new QVBoxLayout(grpUi);
        m_chkShowOutline = new QCheckBox("Show document outline sidebar on startup", grpUi);
        m_chkShowOutline->setChecked(m_cfg.value("show_outline_on_start", true).toBool());
        lUi->addWidget(m_chkShowOutline);

        m_chkSyncDefault = new QCheckBox("Enable scroll synchronization on startup", grpUi);
        m_chkSyncDefault->setChecked(m_cfg.value("sync_scroll_default", true).toBool());
        lUi->addWidget(m_chkSyncDefault);
        lGen->addWidget(grpUi);
        lGen->addStretch();
        tabs->addTab(tabGen, "General");

        // Editor
        auto *tabEd = new QWidget(this);
        auto *lEd = new QVBoxLayout(tabEd);
        auto *grpEdFont = new QGroupBox("Editor Typography", tabEd);
        auto *fEdFont = new QFormLayout(grpEdFont);

        m_comboEdFont = new QFontComboBox(grpEdFont);
        m_comboEdFont->setFontFilters(QFontComboBox::MonospacedFonts);
        m_comboEdFont->setCurrentFont(QFont(m_cfg.value("editor_font_family", "Cascadia Code").toString()));
        fEdFont->addRow("Font Family:", m_comboEdFont);

        m_spinEdFontSize = new QSpinBox(grpEdFont);
        m_spinEdFontSize->setRange(8, 36);
        m_spinEdFontSize->setValue(m_cfg.value("editor_font_size", 11).toInt());
        fEdFont->addRow("Font Size (pt):", m_spinEdFontSize);

        m_spinTabSize = new QSpinBox(grpEdFont);
        m_spinTabSize->setRange(2, 8);
        m_spinTabSize->setValue(m_cfg.value("tab_size", 4).toInt());
        fEdFont->addRow("Tab Indent Spaces:", m_spinTabSize);
        lEd->addWidget(grpEdFont);

        auto *grpEdView = new QGroupBox("Editor Display & Behavior", tabEd);
        auto *lEdView = new QVBoxLayout(grpEdView);
        m_chkLineNumbers = new QCheckBox("Show line numbers in gutter", grpEdView);
        m_chkLineNumbers->setChecked(m_cfg.value("show_line_numbers", true).toBool());
        lEdView->addWidget(m_chkLineNumbers);

        m_chkHighlightLine = new QCheckBox("Highlight current active cursor line", grpEdView);
        m_chkHighlightLine->setChecked(m_cfg.value("highlight_current_line", true).toBool());
        lEdView->addWidget(m_chkHighlightLine);

        m_chkWordWrap = new QCheckBox("Soft wrap long lines at word boundary", grpEdView);
        m_chkWordWrap->setChecked(m_cfg.value("editor_word_wrap", true).toBool());
        lEdView->addWidget(m_chkWordWrap);
        lEd->addWidget(grpEdView);
        lEd->addStretch();
        tabs->addTab(tabEd, "Editor");

        // Reader
        auto *tabRd = new QWidget(this);
        auto *lRd = new QVBoxLayout(tabRd);
        auto *grpRdFont = new QGroupBox("Reader Typography", tabRd);
        auto *fRdFont = new QFormLayout(grpRdFont);

        m_comboRdFont = new QComboBox(grpRdFont);
        m_comboRdFont->addItems({"System Default", "Segoe UI", "Georgia", "Helvetica Neue", "Arial", "Roboto", "Consolas"});
        QString rdVal = m_cfg.value("reader_font_family", "Segoe UI").toString();
        int fIdx = m_comboRdFont->findText(rdVal);
        m_comboRdFont->setCurrentIndex(fIdx != -1 ? fIdx : 1);
        fRdFont->addRow("Document Font:", m_comboRdFont);

        m_spinRdFontSize = new QSpinBox(grpRdFont);
        m_spinRdFontSize->setRange(11, 28);
        m_spinRdFontSize->setValue(m_cfg.value("reader_font_size", 15).toInt());
        fRdFont->addRow("Base Text Size (px):", m_spinRdFontSize);
        lRd->addWidget(grpRdFont);

        auto *grpRender = new QGroupBox("Performance & Rendering", tabRd);
        auto *fRender = new QFormLayout(grpRender);
        m_spinDebounce = new QSpinBox(grpRender);
        m_spinDebounce->setRange(30, 1000);
        m_spinDebounce->setSingleStep(25);
        m_spinDebounce->setSuffix(" ms");
        m_spinDebounce->setValue(m_cfg.value("render_debounce_ms", 100).toInt());
        fRender->addRow("Live Render Delay:", m_spinDebounce);
        lRd->addWidget(grpRender);
        lRd->addStretch();
        tabs->addTab(tabRd, "Reader");

        mainLayout->addWidget(tabs);

        auto *btnBox = new QDialogButtonBox(this);
        auto *btnApply = btnBox->addButton(QDialogButtonBox::Apply);
        btnBox->addButton(QDialogButtonBox::Ok);
        btnBox->addButton(QDialogButtonBox::Cancel);

        connect(btnApply, &QPushButton::clicked, this, [this]() { emit settingsApplied(getValues()); });
        connect(btnBox, &QDialogButtonBox::accepted, this, [this]() {
            emit settingsApplied(getValues());
            accept();
        });
        connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

        mainLayout->addWidget(btnBox);
    }

    QVariantMap m_cfg;
    QComboBox *m_comboTheme;
    QComboBox *m_comboView;
    QCheckBox *m_chkShowOutline;
    QCheckBox *m_chkSyncDefault;
    QFontComboBox *m_comboEdFont;
    QSpinBox *m_spinEdFontSize;
    QSpinBox *m_spinTabSize;
    QCheckBox *m_chkLineNumbers;
    QCheckBox *m_chkHighlightLine;
    QCheckBox *m_chkWordWrap;
    QComboBox *m_comboRdFont;
    QSpinBox *m_spinRdFontSize;
    QSpinBox *m_spinDebounce;
};

class MDReaderApp : public QMainWindow {
    Q_OBJECT
public:
    MDReaderApp(QWidget *parent = nullptr)
        : QMainWindow(parent), m_syncScrollEnabled(true), m_isModified(false), m_syncingScroll(false) {
        setWindowTitle("MD Reader");
        setWindowIcon(getAppWindowIcon());
        resize(1300, 820);

        m_settings = new QSettings("MDReader", "MDReaderApp", this);
        loadSettings();

        m_themeMode = m_config.value("theme_mode", "system").toString();
        m_currentTheme = (m_themeMode == "system") ? detectSystemTheme() : m_themeMode;
        m_lastDetectedTheme = detectSystemTheme();

        m_viewMode = m_config.value("default_view_mode", "split").toString();
        m_syncScrollEnabled = m_config.value("sync_scroll_default", true).toBool();

        m_renderTimer = new QTimer(this);
        m_renderTimer->setSingleShot(true);
        m_renderTimer->setInterval(m_config.value("render_debounce_ms", 100).toInt());
        connect(m_renderTimer, &QTimer::timeout, this, &MDReaderApp::refreshPreviewAndOutline);

        m_themeMonitorTimer = new QTimer(this);
        m_themeMonitorTimer->setInterval(1500);
        connect(m_themeMonitorTimer, &QTimer::timeout, this, &MDReaderApp::checkSystemThemeChanged);
        m_themeMonitorTimer->start();

        initActions();
        initUi();
        applyTheme();
        applyEditorAndPreviewConfig();
        loadInitialContent();
    }

    void loadFile(const QString &path) {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            m_editor->setPlainText(in.readAll());
            file.close();

            m_currentFilePath = path;
            m_isModified = false;
            QString fn = QFileInfo(path).fileName();
            m_lblFile->setText(fn);
            setWindowTitle(QString("MD Reader - %1").arg(fn));
            refreshPreviewAndOutline();
        } else {
            QMessageBox::critical(this, "Read Error", "Unable to read file:\n" + file.errorString());
        }
    }

protected:
    void closeEvent(QCloseEvent *event) override {
        if (maybeSaveChanges()) event->accept();
        else event->ignore();
    }

private:
    void initActions() {
        auto getIcon = [this](const QString &name) {
            return createAppIcon(name, getPalette(m_currentTheme).text_primary, 20);
        };

        act_new = new QAction(getIcon("file_new"), "&New Document", this);
        act_new->setShortcut(QKeySequence::New);
        connect(act_new, &QAction::triggered, this, &MDReaderApp::fileNew);

        act_open = new QAction(getIcon("file_open"), "&Open Document...", this);
        act_open->setShortcut(QKeySequence::Open);
        connect(act_open, &QAction::triggered, this, &MDReaderApp::fileOpen);

        act_save = new QAction(getIcon("file_save"), "&Save Document", this);
        act_save->setShortcut(QKeySequence::Save);
        connect(act_save, &QAction::triggered, this, &MDReaderApp::fileSave);

        act_save_as = new QAction(getIcon("file_save_as"), "Save Document &As...", this);
        act_save_as->setShortcut(QKeySequence::SaveAs);
        connect(act_save_as, &QAction::triggered, this, &MDReaderApp::fileSaveAs);

        act_export_html = new QAction(getIcon("export_html"), "Export to Standalone &HTML...", this);
        connect(act_export_html, &QAction::triggered, this, &MDReaderApp::exportHtml);

        act_export_pdf = new QAction(getIcon("export_pdf"), "Print / Export to &PDF...", this);
        act_export_pdf->setShortcut(QKeySequence("Ctrl+P"));
        connect(act_export_pdf, &QAction::triggered, this, &MDReaderApp::exportPdf);

        act_settings = new QAction(getIcon("settings"), "Se&ttings...", this);
        act_settings->setShortcut(QKeySequence::Preferences);
        connect(act_settings, &QAction::triggered, this, &MDReaderApp::openSettings);

        act_exit = new QAction(getIcon("exit"), "E&xit MD Reader", this);
        act_exit->setShortcut(QKeySequence::Quit);
        connect(act_exit, &QAction::triggered, this, &QWidget::close);

        act_undo = new QAction(getIcon("undo"), "&Undo", this);
        act_undo->setShortcut(QKeySequence::Undo);

        act_redo = new QAction(getIcon("redo"), "&Redo", this);
        act_redo->setShortcut(QKeySequence::Redo);

        act_find = new QAction(getIcon("find"), "&Find & Replace", this);
        act_find->setShortcut(QKeySequence::Find);
        connect(act_find, &QAction::triggered, this, &MDReaderApp::toggleFindBar);

        act_select_all = new QAction(getIcon("select_all"), "Select &All", this);
        act_select_all->setShortcut(QKeySequence::SelectAll);

        act_bold = new QAction(getIcon("bold"), "Bold", this);
        act_bold->setShortcut(QKeySequence("Ctrl+B"));
        connect(act_bold, &QAction::triggered, this, [this]() { insertMarkdownTag("**", "**"); });

        act_italic = new QAction(getIcon("italic"), "Italic", this);
        act_italic->setShortcut(QKeySequence("Ctrl+I"));
        connect(act_italic, &QAction::triggered, this, [this]() { insertMarkdownTag("*", "*"); });

        act_strike = new QAction(getIcon("strikethrough"), "Strikethrough", this);
        connect(act_strike, &QAction::triggered, this, [this]() { insertMarkdownTag("~~", "~~"); });

        act_code = new QAction(getIcon("code"), "Code Block", this);
        connect(act_code, &QAction::triggered, this, &MDReaderApp::insertCodeBlock);

        act_quote = new QAction(getIcon("quote"), "Blockquote", this);
        connect(act_quote, &QAction::triggered, this, [this]() { insertMarkdownPrefix("> "); });

        act_list = new QAction(getIcon("list_bullet"), "Bullet List", this);
        connect(act_list, &QAction::triggered, this, [this]() { insertMarkdownPrefix("- "); });

        act_task = new QAction(getIcon("list_task"), "Task Checklist", this);
        connect(act_task, &QAction::triggered, this, [this]() { insertMarkdownPrefix("- [ ] "); });

        act_table = new QAction(getIcon("table"), "Table Template", this);
        connect(act_table, &QAction::triggered, this, &MDReaderApp::insertMarkdownTable);

        act_link = new QAction(getIcon("link"), "Hyperlink", this);
        connect(act_link, &QAction::triggered, this, [this]() { insertMarkdownTag("[", "](https://)"); });

        act_split = new QAction(getIcon("split"), "Side-by-Side (Split)", this);
        act_split->setCheckable(true);
        connect(act_split, &QAction::triggered, this, [this]() { setViewMode("split"); });

        act_reader = new QAction(getIcon("reader"), "Reader Only", this);
        act_reader->setCheckable(true);
        connect(act_reader, &QAction::triggered, this, [this]() { setViewMode("preview"); });

        act_editor = new QAction(getIcon("editor"), "Editor Only", this);
        act_editor->setCheckable(true);
        connect(act_editor, &QAction::triggered, this, [this]() { setViewMode("editor"); });

        act_sync = new QAction(getIcon("sync_on"), "Synchronize Scrolling", this);
        act_sync->setCheckable(true);
        act_sync->setChecked(m_syncScrollEnabled);
        connect(act_sync, &QAction::toggled, this, &MDReaderApp::toggleSyncScroll);

        act_theme = new QAction(getIcon("theme_dark"), "Toggle Theme", this);
        act_theme->setShortcut(QKeySequence("Ctrl+T"));
        connect(act_theme, &QAction::triggered, this, &MDReaderApp::toggleTheme);

        act_sidebar = new QAction(getIcon("sidebar"), "Toggle Outline Sidebar", this);
        connect(act_sidebar, &QAction::triggered, this, &MDReaderApp::toggleSidebar);

        act_zoom_in = new QAction(getIcon("zoom_in"), "Zoom In", this);
        act_zoom_in->setShortcut(QKeySequence::ZoomIn);

        act_zoom_out = new QAction(getIcon("zoom_out"), "Zoom Out", this);
        act_zoom_out->setShortcut(QKeySequence::ZoomOut);

        act_zoom_reset = new QAction(getIcon("zoom_reset"), "Reset Zoom", this);
    }

    void initUi() {
        m_mainSplitter = new QSplitter(Qt::Horizontal, this);
        setCentralWidget(m_mainSplitter);

        // Sidebar
        m_sidebarPanel = new QFrame(this);
        m_sidebarPanel->setMaximumWidth(280);
        m_sidebarPanel->setMinimumWidth(180);
        m_sidebarPanel->setFrameShape(QFrame::NoFrame);
        auto *sLayout = new QVBoxLayout(m_sidebarPanel);
        sLayout->setContentsMargins(6, 6, 6, 6);
        sLayout->setSpacing(6);

        auto *lblTitle = new QLabel("DOCUMENT OUTLINE", m_sidebarPanel);
        lblTitle->setObjectName("sidebarTitle");
        sLayout->addWidget(lblTitle);

        m_outlineWidget = new OutlineWidget(m_sidebarPanel);
        connect(m_outlineWidget, &OutlineWidget::headingClicked, this, &MDReaderApp::jumpToEditorLine);
        sLayout->addWidget(m_outlineWidget);
        m_mainSplitter->addWidget(m_sidebarPanel);

        // Content Area
        m_contentSplitter = new QSplitter(Qt::Horizontal, this);

        m_editorContainer = new QWidget(this);
        auto *edLayout = new QVBoxLayout(m_editorContainer);
        edLayout->setContentsMargins(0, 0, 0, 0);
        edLayout->setSpacing(0);

        m_findBar = new FindReplaceBar(m_currentTheme, this);
        connect(m_findBar, &FindReplaceBar::findNextSignal, this, &MDReaderApp::findInEditor);
        connect(m_findBar, &FindReplaceBar::findPrevSignal, this, &MDReaderApp::findPrevInEditor);
        connect(m_findBar, &FindReplaceBar::replaceSignal, this, &MDReaderApp::replaceInEditor);
        connect(m_findBar, &FindReplaceBar::replaceAllSignal, this, &MDReaderApp::replaceAllInEditor);
        edLayout->addWidget(m_findBar);

        m_editor = new CodeEditor(m_currentTheme, this);
        connect(m_editor, &CodeEditor::textChanged, this, &MDReaderApp::onEditorTextChanged);
        connect(m_editor, &CodeEditor::cursorPositionChanged, this, &MDReaderApp::updateStatusBarStats);
        connect(m_editor->verticalScrollBar(), &QScrollBar::valueChanged, this, &MDReaderApp::onEditorScrollChanged);
        edLayout->addWidget(m_editor);

        connect(act_undo, &QAction::triggered, m_editor, &QPlainTextEdit::undo);
        connect(act_redo, &QAction::triggered, m_editor, &QPlainTextEdit::redo);
        connect(act_select_all, &QAction::triggered, m_editor, &QPlainTextEdit::selectAll);

        m_contentSplitter->addWidget(m_editorContainer);

        m_preview = new MarkdownPreviewWidget(m_currentTheme, this);
        connect(m_preview, &MarkdownPreviewWidget::scrolledByUser, this, &MDReaderApp::onPreviewScrollChanged);
        m_contentSplitter->addWidget(m_preview);

        connect(act_zoom_in, &QAction::triggered, m_preview, &MarkdownPreviewWidget::zoomInPreview);
        connect(act_zoom_out, &QAction::triggered, m_preview, &MarkdownPreviewWidget::zoomOutPreview);
        connect(act_zoom_reset, &QAction::triggered, m_preview, &MarkdownPreviewWidget::zoomResetPreview);

        m_mainSplitter->addWidget(m_contentSplitter);
        m_mainSplitter->setStretchFactor(0, 0);
        m_mainSplitter->setStretchFactor(1, 1);
        m_contentSplitter->setStretchFactor(0, 1);
        m_contentSplitter->setStretchFactor(1, 1);

        createToolbars();
        createMenus();
        createStatusBarWidget();

        new QShortcut(QKeySequence("Esc"), this, [this]() { m_findBar->hide(); });
    }

    void createToolbars() {
        m_toolbar = addToolBar("Main Controls");
        m_toolbar->setMovable(false);
        m_toolbar->setIconSize(QSize(18, 18));

        m_toolbar->addAction(act_new);
        m_toolbar->addAction(act_open);
        m_toolbar->addAction(act_save);
        m_toolbar->addSeparator();

        m_toolbar->addAction(act_bold);
        m_toolbar->addAction(act_italic);
        m_toolbar->addAction(act_strike);
        m_toolbar->addAction(act_code);
        m_toolbar->addAction(act_quote);
        m_toolbar->addAction(act_list);
        m_toolbar->addAction(act_task);
        m_toolbar->addAction(act_table);
        m_toolbar->addAction(act_link);
        m_toolbar->addSeparator();

        m_toolbar->addAction(act_split);
        m_toolbar->addAction(act_reader);
        m_toolbar->addAction(act_editor);
        m_toolbar->addSeparator();

        m_toolbar->addAction(act_sync);
        m_toolbar->addAction(act_theme);
        m_toolbar->addAction(act_sidebar);
        m_toolbar->addSeparator();

        m_toolbar->addAction(act_zoom_in);
        m_toolbar->addAction(act_zoom_out);
        m_toolbar->addAction(act_zoom_reset);
    }

    void createMenus() {
        auto *mb = menuBar();

        auto *fileMenu = mb->addMenu("&File");
        fileMenu->addAction(act_new);
        fileMenu->addAction(act_open);
        fileMenu->addSeparator();
        fileMenu->addAction(act_save);
        fileMenu->addAction(act_save_as);
        fileMenu->addSeparator();
        fileMenu->addAction(act_export_html);
        fileMenu->addAction(act_export_pdf);
        fileMenu->addSeparator();
        fileMenu->addAction(act_settings);
        fileMenu->addSeparator();
        fileMenu->addAction(act_exit);

        auto *editMenu = mb->addMenu("&Edit");
        editMenu->addAction(act_undo);
        editMenu->addAction(act_redo);
        editMenu->addSeparator();
        editMenu->addAction(act_bold);
        editMenu->addAction(act_italic);
        editMenu->addAction(act_strike);
        editMenu->addAction(act_code);
        editMenu->addAction(act_table);
        editMenu->addAction(act_link);
        editMenu->addSeparator();
        editMenu->addAction(act_find);
        editMenu->addAction(act_select_all);

        auto *viewMenu = mb->addMenu("&View");
        viewMenu->addAction(act_split);
        viewMenu->addAction(act_reader);
        viewMenu->addAction(act_editor);
        viewMenu->addSeparator();
        viewMenu->addAction(act_sync);
        viewMenu->addAction(act_theme);
        viewMenu->addAction(act_sidebar);
        viewMenu->addSeparator();
        viewMenu->addAction(act_zoom_in);
        viewMenu->addAction(act_zoom_out);
        viewMenu->addAction(act_zoom_reset);
    }

    void createStatusBarWidget() {
        m_status = statusBar();
        m_lblFile = new QLabel("Welcome.md", this);
        m_lblStats = new QLabel("Words: 0 | Chars: 0 | Lines: 1 | Read: ~0 min", this);
        m_lblCursor = new QLabel("Ln 1, Col 1", this);
        m_lblSync = new QLabel("Sync: ON", this);
        m_lblMode = new QLabel("Mode: Split", this);

        m_status->addWidget(m_lblFile);
        m_status->addPermanentWidget(m_lblStats);
        m_status->addPermanentWidget(m_lblCursor);
        m_status->addPermanentWidget(m_lblSync);
        m_status->addPermanentWidget(m_lblMode);
    }

    void updateStatusBarStats() {
        QString text = m_editor->toPlainText();
        int words = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).size();
        int chars = text.length();
        int lines = m_editor->blockCount();
        int readingTime = std::ceil(words / 200.0);

        m_lblStats->setText(QString("Words: %1 | Chars: %2 | Lines: %3 | Read: ~%4 min")
                            .arg(words).arg(chars).arg(lines).arg(readingTime));

        auto cursor = m_editor->textCursor();
        m_lblCursor->setText(QString("Ln %1, Col %2")
                            .arg(cursor.blockNumber() + 1)
                            .arg(cursor.positionInBlock() + 1));
    }

    void toggleSyncScroll(bool enabled) {
        m_syncScrollEnabled = enabled;
        m_lblSync->setText(enabled ? "Sync: ON" : "Sync: OFF");
        act_sync->setChecked(enabled);
        act_sync->setIcon(createAppIcon(enabled ? "sync_on" : "sync_off", getPalette(m_currentTheme).text_primary, 20));
    }

    void onEditorScrollChanged(int value) {
        if (!m_syncScrollEnabled || m_syncingScroll || m_viewMode != "split") return;
        m_syncingScroll = true;
        auto *sb = m_editor->verticalScrollBar();
        qreal ratio = (sb->maximum() > 0) ? static_cast<qreal>(value) / sb->maximum() : 0.0;
        m_preview->scrollToRatio(ratio);
        m_syncingScroll = false;
    }

    void onPreviewScrollChanged(qreal ratio) {
        if (!m_syncScrollEnabled || m_syncingScroll || m_viewMode != "split") return;
        m_syncingScroll = true;
        auto *sb = m_editor->verticalScrollBar();
        sb->setValue(qRound(ratio * sb->maximum()));
        m_syncingScroll = false;
    }

    void insertMarkdownTag(const QString &prefix, const QString &suffix) {
        auto cursor = m_editor->textCursor();
        if (cursor.hasSelection()) {
            cursor.insertText(prefix + cursor.selectedText() + suffix);
        } else {
            cursor.insertText(prefix + "text" + suffix);
            int pos = cursor.position() - suffix.length();
            cursor.setPosition(pos - 4);
            cursor.setPosition(pos, QTextCursor::KeepAnchor);
            m_editor->setTextCursor(cursor);
        }
        m_editor->setFocus();
    }

    void insertMarkdownPrefix(const QString &prefix) {
        auto cursor = m_editor->textCursor();
        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.insertText(prefix);
        m_editor->setFocus();
    }

    void insertCodeBlock() {
        auto cursor = m_editor->textCursor();
        if (cursor.hasSelection()) {
            cursor.insertText(QString("```cpp\n%1\n```\n").arg(cursor.selectedText()));
        } else {
            cursor.insertText("```cpp\n// Insert code here\n```\n");
        }
        m_editor->setFocus();
    }

    void insertMarkdownTable() {
        const QString table =
            "\n| Header 1 | Header 2 | Header 3 |\n"
            "| :--- | :---: | ---: |\n"
            "| Item 1 | Item 2 | Item 3 |\n"
            "| Data A | Data B | Data C |\n\n";
        m_editor->textCursor().insertText(table);
        m_editor->setFocus();
    }

    void setViewMode(const QString &mode) {
        m_viewMode = mode;
        act_split->setChecked(mode == "split");
        act_reader->setChecked(mode == "preview");
        act_editor->setChecked(mode == "editor");

        if (mode == "editor") {
            m_editorContainer->setVisible(true);
            m_preview->setVisible(false);
            m_lblMode->setText("Mode: Editor");
        } else if (mode == "preview") {
            m_editorContainer->setVisible(false);
            m_preview->setVisible(true);
            m_lblMode->setText("Mode: Reader");
        } else {
            m_editorContainer->setVisible(true);
            m_preview->setVisible(true);
            m_contentSplitter->setSizes({width() / 2, width() / 2});
            m_lblMode->setText("Mode: Split");
        }
    }

    void toggleSidebar() {
        m_sidebarPanel->setVisible(!m_sidebarPanel->isVisible());
    }

    void checkSystemThemeChanged() {
        if (m_themeMode != "system") return;
        QString detected = detectSystemTheme();
        if (detected != m_lastDetectedTheme) {
            m_lastDetectedTheme = detected;
            m_currentTheme = detected;
            applyTheme();
        }
    }

    void toggleTheme() {
        m_currentTheme = (m_currentTheme == "dark") ? "light" : "dark";
        m_themeMode = m_currentTheme;
        m_config["theme_mode"] = m_currentTheme;
        saveSettings();
        m_lastDetectedTheme = m_currentTheme;
        applyTheme();
    }

    void openSettings() {
        SettingsDialog dlg(m_config, this);
        connect(&dlg, &SettingsDialog::settingsApplied, this, &MDReaderApp::applySettings);
        dlg.exec();
    }

    void applySettings(const QVariantMap &newConfig) {
        for (auto it = newConfig.begin(); it != newConfig.end(); ++it) {
            m_config[it.key()] = it.value();
        }
        saveSettings();

        m_themeMode = m_config.value("theme_mode", "system").toString();
        m_currentTheme = (m_themeMode == "system") ? detectSystemTheme() : m_themeMode;
        m_renderTimer->setInterval(m_config.value("render_debounce_ms", 100).toInt());

        applyEditorAndPreviewConfig();
        applyTheme();
    }

    void applyEditorAndPreviewConfig() {
        QString edFont = m_config.value("editor_font_family", "Cascadia Code").toString();
        int edSize = m_config.value("editor_font_size", 11).toInt();
        m_editor->setEditorFont(edFont, edSize);
        m_editor->setTabSpaces(m_config.value("tab_size", 4).toInt());
        m_editor->setLineNumbersVisible(m_config.value("show_line_numbers", true).toBool());
        m_editor->setLineHighlighting(m_config.value("highlight_current_line", true).toBool());

        bool wrap = m_config.value("editor_word_wrap", true).toBool();
        m_editor->setWordWrapMode(wrap ? QTextOption::WrapAtWordBoundaryOrAnywhere : QTextOption::NoWrap);

        QString rdFont = m_config.value("reader_font_family", "Segoe UI").toString();
        int rdSize = m_config.value("reader_font_size", 15).toInt();
        m_preview->setPreviewTypography(rdFont, rdSize, m_editor->toPlainText());
    }

    void applyTheme() {
        setStyleSheet(generateQss(m_currentTheme));
        m_editor->setTheme(m_currentTheme);
        m_findBar->setTheme(m_currentTheme);
        m_preview->setTheme(m_currentTheme, m_editor->toPlainText());
        updateActionIcons();
    }

    void updateActionIcons() {
        auto getIcon = [this](const QString &name) {
            return createAppIcon(name, getPalette(m_currentTheme).text_primary, 20);
        };

        act_new->setIcon(getIcon("file_new"));
        act_open->setIcon(getIcon("file_open"));
        act_save->setIcon(getIcon("file_save"));
        act_save_as->setIcon(getIcon("file_save_as"));
        act_export_html->setIcon(getIcon("export_html"));
        act_export_pdf->setIcon(getIcon("export_pdf"));
        act_settings->setIcon(getIcon("settings"));
        act_exit->setIcon(getIcon("exit"));

        act_undo->setIcon(getIcon("undo"));
        act_redo->setIcon(getIcon("redo"));
        act_find->setIcon(getIcon("find"));
        act_select_all->setIcon(getIcon("select_all"));

        act_bold->setIcon(getIcon("bold"));
        act_italic->setIcon(getIcon("italic"));
        act_strike->setIcon(getIcon("strikethrough"));
        act_code->setIcon(getIcon("code"));
        act_quote->setIcon(getIcon("quote"));
        act_list->setIcon(getIcon("list_bullet"));
        act_task->setIcon(getIcon("list_task"));
        act_table->setIcon(getIcon("table"));
        act_link->setIcon(getIcon("link"));

        act_split->setIcon(getIcon("split"));
        act_reader->setIcon(getIcon("reader"));
        act_editor->setIcon(getIcon("editor"));
        act_sync->setIcon(getIcon(m_syncScrollEnabled ? "sync_on" : "sync_off"));
        act_theme->setIcon(getIcon((m_currentTheme == "dark") ? "theme_dark" : "theme_light"));
        act_sidebar->setIcon(getIcon("sidebar"));
        act_zoom_in->setIcon(getIcon("zoom_in"));
        act_zoom_out->setIcon(getIcon("zoom_out"));
        act_zoom_reset->setIcon(getIcon("zoom_reset"));
    }

    void onEditorTextChanged() {
        m_isModified = true;
        QString fn = m_currentFilePath.isEmpty() ? "Untitled.md" : QFileInfo(m_currentFilePath).fileName();
        m_lblFile->setText(fn + "*");
        setWindowTitle(QString("MD Reader - %1*").arg(fn));
        m_renderTimer->start();
    }

    void refreshPreviewAndOutline() {
        QString text = m_editor->toPlainText();
        m_preview->setContent(text);
        m_outlineWidget->updateOutline(text);
        updateStatusBarStats();
    }

    void jumpToEditorLine(int lineNum) {
        QTextBlock block = m_editor->document()->findBlockByNumber(lineNum);
        if (block.isValid()) {
            QTextCursor cursor(block);
            m_editor->setTextCursor(cursor);
            m_editor->centerCursor();
            m_editor->setFocus();
            if (m_syncScrollEnabled && m_viewMode == "split") {
                auto *sb = m_editor->verticalScrollBar();
                qreal ratio = (sb->maximum() > 0) ? static_cast<qreal>(sb->value()) / sb->maximum() : 0.0;
                m_preview->scrollToRatio(ratio);
            }
        }
    }

    void toggleFindBar() {
        m_findBar->setVisible(!m_findBar->isVisible());
        if (m_findBar->isVisible()) {
            m_findBar->findInput()->setFocus();
            m_findBar->findInput()->selectAll();
        }
    }

    void findInEditor(const QString &query, bool matchCase) {
        QTextDocument::FindFlags flags;
        if (matchCase) flags |= QTextDocument::FindCaseSensitively;
        bool found = m_editor->find(query, flags);
        if (!found) {
            auto cur = m_editor->textCursor();
            cur.movePosition(QTextCursor::Start);
            m_editor->setTextCursor(cur);
            m_editor->find(query, flags);
        }
    }

    void findPrevInEditor(const QString &query, bool matchCase) {
        QTextDocument::FindFlags flags = QTextDocument::FindBackward;
        if (matchCase) flags |= QTextDocument::FindCaseSensitively;
        bool found = m_editor->find(query, flags);
        if (!found) {
            auto cur = m_editor->textCursor();
            cur.movePosition(QTextCursor::End);
            m_editor->setTextCursor(cur);
            m_editor->find(query, flags);
        }
    }

    void replaceInEditor(const QString &search, const QString &replaceWith) {
        auto cursor = m_editor->textCursor();
        if (cursor.hasSelection() && cursor.selectedText() == search) {
            cursor.insertText(replaceWith);
        }
        findInEditor(search, m_findBar->isMatchCase());
    }

    void replaceAllInEditor(const QString &search, const QString &replaceWith) {
        if (search.isEmpty()) return;
        QString text = m_editor->toPlainText();
        QString newText = text;
        newText.replace(search, replaceWith);
        if (text != newText) m_editor->setPlainText(newText);
    }

    void fileNew() {
        if (maybeSaveChanges()) {
            m_editor->clear();
            m_currentFilePath.clear();
            m_isModified = false;
            m_lblFile->setText("Untitled.md");
            setWindowTitle("MD Reader - Untitled.md");
            refreshPreviewAndOutline();
        }
    }

    void fileOpen() {
        if (!maybeSaveChanges()) return;
        QString path = QFileDialog::getOpenFileName(
            this, "Open Markdown Document", "",
            "Markdown Files (*.md *.markdown *.mdown);;Text Files (*.txt);;All Files (*)"
        );
        if (!path.isEmpty()) loadFile(path);
    }

    bool fileSave() {
        if (m_currentFilePath.isEmpty()) return fileSaveAs();
        return saveToPath(m_currentFilePath);
    }

    bool fileSaveAs() {
        QString path = QFileDialog::getSaveFileName(
            this, "Save Markdown Document", "",
            "Markdown Files (*.md);;All Files (*)"
        );
        if (!path.isEmpty()) return saveToPath(path);
        return false;
    }

    bool saveToPath(const QString &path) {
        QFile file(path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << m_editor->toPlainText();
            file.close();

            m_currentFilePath = path;
            m_isModified = false;
            QString fn = QFileInfo(path).fileName();
            m_lblFile->setText(fn);
            setWindowTitle(QString("MD Reader - %1").arg(fn));
            return true;
        }
        QMessageBox::critical(this, "Save Error", "Could not save file:\n" + file.errorString());
        return false;
    }

    bool maybeSaveChanges() {
        if (!m_isModified) return true;
        auto reply = QMessageBox::question(
            this, "Unsaved Changes",
            "The document has been modified. Would you like to save changes?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save
        );
        if (reply == QMessageBox::Save) return fileSave();
        if (reply == QMessageBox::Cancel) return false;
        return true;
    }

    void exportHtml() {
        QString path = QFileDialog::getSaveFileName(this, "Export to Standalone HTML", "", "HTML Files (*.html)");
        if (!path.isEmpty()) {
            QString htmlContent = convertMarkdownToHtml(m_editor->toPlainText(), m_currentTheme);
            QFile file(path);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << htmlContent;
                file.close();
                QMessageBox::information(this, "Export Complete", "Exported successfully to:\n" + path);
            } else {
                QMessageBox::critical(this, "Export Error", "Failed to export HTML:\n" + file.errorString());
            }
        }
    }

    void exportPdf() {
        QPrinter printer(QPrinter::HighResolution);
        QPrintDialog dialog(&printer, this);
        if (dialog.exec() == QDialog::Accepted) {
            QTextDocument doc;
            doc.setHtml(convertMarkdownToHtml(m_editor->toPlainText(), "light"));
            doc.print(&printer);
        }
    }

    void loadSettings() {
        m_config["theme_mode"] = m_settings->value("theme_mode", "system").toString();
        m_config["default_view_mode"] = m_settings->value("default_view_mode", "split").toString();
        m_config["show_outline_on_start"] = m_settings->value("show_outline_on_start", true).toBool();
        m_config["sync_scroll_default"] = m_settings->value("sync_scroll_default", true).toBool();
        m_config["editor_font_family"] = m_settings->value("editor_font_family", "Cascadia Code").toString();
        m_config["editor_font_size"] = m_settings->value("editor_font_size", 11).toInt();
        m_config["tab_size"] = m_settings->value("tab_size", 4).toInt();
        m_config["show_line_numbers"] = m_settings->value("show_line_numbers", true).toBool();
        m_config["highlight_current_line"] = m_settings->value("highlight_current_line", true).toBool();
        m_config["editor_word_wrap"] = m_settings->value("editor_word_wrap", true).toBool();
        m_config["reader_font_family"] = m_settings->value("reader_font_family", "Segoe UI").toString();
        m_config["reader_font_size"] = m_settings->value("reader_font_size", 15).toInt();
        m_config["render_debounce_ms"] = m_settings->value("render_debounce_ms", 100).toInt();
    }

    void saveSettings() {
        for (auto it = m_config.begin(); it != m_config.end(); ++it) {
            m_settings->setValue(it.key(), it.value());
        }
    }

    void loadInitialContent() {
        const QString demo =
            "# Welcome to MD Reader (C++ Edition)\n\n"
            "A high-performance Markdown studio featuring side-by-side editing, live preview, and synchronization.\n\n"
            "---\n\n"
            "## Capabilities & Architecture\n\n"
            "- **Native C++ Performance**: Built purely with modern Qt 6 / Qt 5 and C++17.\n"
            "- **Side-by-Side Dual View**: Work seamlessly with the Markdown editor on the left and rendered reader on the right.\n"
            "- **Synchronized Scrolling**: Toggle synchronized scroll on the toolbar to lock viewports together.\n"
            "- **Custom Vector Graphics**: Pixel-perfect high-DPI scaling powered by QPainter.\n"
            "- **Document Navigation**: Interactive table of contents outline parsing `#` headers.\n\n"
            "---\n\n"
            "## Code Blocks & Highlights\n\n"
            "```cpp\n"
            "#include <iostream>\n\n"
            "int main() {\n"
            "    std::cout << \"Hello from MD Reader C++!\" << std::endl;\n"
            "    return 0;\n"
            "}\n"
            "```\n\n"
            "### Quotes & Checklist\n\n"
            "> \"Good design is as little design as possible. Less, but better.\"\n"
            "> - *Dieter Rams*\n\n"
            "- [x] Native C++ Qt architecture\n"
            "- [x] Vector sapphire notebook & stylus logo\n"
            "- [x] Windows auto dark/light theme detection\n"
            "- [x] Synchronized dual pane scroll lock\n"
            "- [ ] Write your next project document\n";

        m_editor->setPlainText(demo);
        m_isModified = false;
        m_lblFile->setText("Welcome.md");
        refreshPreviewAndOutline();
    }

    // State Variables
    QString m_currentFilePath;
    QString m_themeMode;
    QString m_currentTheme;
    QString m_lastDetectedTheme;
    QString m_viewMode;
    bool m_syncScrollEnabled;
    bool m_isModified;
    bool m_syncingScroll;
    QVariantMap m_config;
    QSettings *m_settings;
    QTimer *m_renderTimer;
    QTimer *m_themeMonitorTimer;

    // UI Widgets
    QSplitter *m_mainSplitter;
    QSplitter *m_contentSplitter;
    QFrame *m_sidebarPanel;
    OutlineWidget *m_outlineWidget;
    QWidget *m_editorContainer;
    FindReplaceBar *m_findBar;
    CodeEditor *m_editor;
    MarkdownPreviewWidget *m_preview;
    QToolBar *m_toolbar;
    QStatusBar *m_status;
    QLabel *m_lblFile;
    QLabel *m_lblStats;
    QLabel *m_lblCursor;
    QLabel *m_lblSync;
    QLabel *m_lblMode;

    // Actions
    QAction *act_new;
    QAction *act_open;
    QAction *act_save;
    QAction *act_save_as;
    QAction *act_export_html;
    QAction *act_export_pdf;
    QAction *act_settings;
    QAction *act_exit;
    QAction *act_undo;
    QAction *act_redo;
    QAction *act_find;
    QAction *act_select_all;
    QAction *act_bold;
    QAction *act_italic;
    QAction *act_strike;
    QAction *act_code;
    QAction *act_quote;
    QAction *act_list;
    QAction *act_task;
    QAction *act_table;
    QAction *act_link;
    QAction *act_split;
    QAction *act_reader;
    QAction *act_editor;
    QAction *act_sync;
    QAction *act_theme;
    QAction *act_sidebar;
    QAction *act_zoom_in;
    QAction *act_zoom_out;
    QAction *act_zoom_reset;
};

int main(int argc, char *argv[]) {
#ifdef Q_OS_WIN
    SetCurrentProcessExplicitAppUserModelID(L"gemini.mdreader.cpp.application");
#endif

    QApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough
    );
    QApplication app(argc, argv);
    app.setWindowIcon(getAppWindowIcon());
    app.setStyle("Fusion");
    app.setApplicationName("MD Reader");
    app.setOrganizationName("MDReader");

    MDReaderApp window;

    // Open file passed by Windows Explorer (double click / right click context menu)
    if (argc > 1) {
        QString targetFile = QString::fromLocal8Bit(argv[1]);
        if (QFile::exists(targetFile)) {
            window.loadFile(targetFile);
        }
    }

    window.show();

    return app.exec();
}

#include "main.moc"