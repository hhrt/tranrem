#ifndef SETTINGS_H
#define SETTINGS_H
#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QString;

class SettingsDialog : public QDialog
{

  Q_OBJECT

  public:
  SettingsDialog(QString h = NULL, QString p = NULL, QString u = NULL, QWidget *parent = 0);

  signals:
  void applyed(QString h, QString p, QString u);
  void canceled();

  private:
  QLabel *hostLabel;
  QLabel *portLabel;
  QLabel *urlLabel;
  QLineEdit *hostLineEdit;
  QLineEdit *portLineEdit;
  QLineEdit *urlLineEdit;
  QPushButton *okButton;
  QPushButton *cancelButton;
  QString host;
  QString port;
  QString url;

  private slots:
  void okButtonClicked();
  void cancelButtonClicked();
  void hostChanged(QString h);
  void portChanged(QString p);
  void urlChanged(QString u);

};

#endif
