#ifndef PreciseLandingAssistCard_H
#define PreciseLandingAssistCard_H

#include "precise_landing_assist_ctrl.h"


namespace solo
{

class PreciseLandingAssistCard : public QWidget
{
    Q_OBJECT

private:
    typedef std::function<void (PreciseLandingAssistCard *, const QJsonValue &)>   set_func;

public:
    PreciseLandingAssistCard(QWidget *parent = nullptr);
    ~PreciseLandingAssistCard() override;

    void set_idsn(const QString &idsn);
    QString idsn() const;

    void set_card_struct_data(eqnx_dh::CardContentItem *cardContentItem);
    void set_state_data(const QJsonObject &jo);

    QStringList uri_roots() const;

    void set_platform_longitude(const QJsonValue &val);
    void set_platform_latitude(const QJsonValue &val);

    void set_uav_longitude(const QJsonValue &val);
    void set_uav_latitude(const QJsonValue &val);

private:
    void init_members();
    void init_ui();
    void init_signal_slots();

    void init_timers();

protected:
    void resizeEvent(QResizeEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;

private:
    GLPoint2f calc_uav_pos() const;

private:
    QJsonValue parse_data_by_uri(const QJsonObject &jo, QStringList &uri);

private slots:
    void tm_update_slot();

private:
    PreciseLandingAssistCtrl    *_ctrl;

private:
    double      _platform_lon;
    double      _platform_lat;
    double      _uav_lon;
    double      _uav_lat;

    QString     _idsn;

    QHash<QString, QStringList>  _hash_name_uris;

private:
    // assist vars
    static QHash<QString, set_func>    hash_name_func;

    QStringList _list_uri_roots;

    QTimer      _tm_update;

private:
    QPoint      _pt_offset;

};

}   // solo

#endif // PreciseLandingAssistCard_H
