#include "precise_landing_assist_card.h"

#include "aosk_algorithms_export_global.h"


namespace solo
{

QHash<QString, PreciseLandingAssistCard::set_func> PreciseLandingAssistCard::hash_name_func;


PreciseLandingAssistCard::PreciseLandingAssistCard(QWidget *parent)
    : cs::CSWidget(parent)
{
    init_members();
    init_ui();
    init_signal_slots();

    init_timers();
}

PreciseLandingAssistCard::~PreciseLandingAssistCard()
{
    DELETE_Q_POINTER(_ctrl);
}

void PreciseLandingAssistCard::set_idsn(const QString &idsn)
{
    _idsn = idsn;
}

QString PreciseLandingAssistCard::idsn() const
{
    return _idsn;
}

void PreciseLandingAssistCard::set_card_struct_data(eqnx_dh::CardContentItem *cardContentItem)
{
    // Clear container
    _hash_name_uris.clear();

    // Parse json
    if (!cardContentItem) return;

    // Validate the card visual type
    // to modify
    if (cardContentItem->vslType != 1) return;

    for (auto itemElem : cardContentItem->listItem)
    {
        // Set title
        auto listKidItems = itemElem->listItem;
        for (auto kidItem : listKidItems)
        {
            auto varDef = kidItem->definition;

            // split the uri
            auto listUris = varDef->uri().split("/");

            // Ignore sense case
            auto lowerName = varDef->name().toLower();

            _hash_name_uris.insert(varDef->name(), listUris);
        }
    }
}

void PreciseLandingAssistCard::set_state_data(const QJsonObject &jo)
{
    for (auto name : _hash_name_uris.keys())
    {
        // get data
        auto uri = _hash_name_uris.value(name);
        auto data = parse_data_by_uri(jo, uri);
        if (data.isNull()) continue;

        // call function to set data
        if (!hash_name_func.contains(name)) continue;

        auto func = hash_name_func.value(name);
        func(this, data);
    }
}

QStringList PreciseLandingAssistCard::uri_roots() const
{
    return _list_uri_roots;
}

void PreciseLandingAssistCard::set_platform_longitude(const QJsonValue &val)
{
    DROP_ABNORMAL_DATA(val.toDouble());
    _platform_lon = val.toDouble();
}

void PreciseLandingAssistCard::set_platform_latitude(const QJsonValue &val)
{
    DROP_ABNORMAL_DATA(val.toDouble());
    _platform_lat = val.toDouble();
}

void PreciseLandingAssistCard::set_uav_longitude(const QJsonValue &val)
{
    DROP_ABNORMAL_DATA(val.toDouble());
    _uav_lon = val.toDouble();
}

void PreciseLandingAssistCard::set_uav_latitude(const QJsonValue &val)
{
    DROP_ABNORMAL_DATA(val.toDouble());
    _uav_lat = val.toDouble();
}

void PreciseLandingAssistCard::init_members()
{
    // initialize the name/function hash
    if (hash_name_func.isEmpty())
    {
        hash_name_func.insert("platform_center_lon",    &PreciseLandingAssistCard::set_platform_longitude);
        hash_name_func.insert("platform_center_lat",    &PreciseLandingAssistCard::set_platform_latitude);
        hash_name_func.insert("uav_lon",                &PreciseLandingAssistCard::set_uav_longitude);
        hash_name_func.insert("uav_lat",                &PreciseLandingAssistCard::set_uav_latitude);
    }

    _platform_lon = 0;
    _platform_lat = 0;
    _uav_lon = 0;
    _uav_lat = 0;

    _ctrl = new PreciseLandingAssistCtrl(this);
}

void PreciseLandingAssistCard::init_ui()
{
    setCursor(Qt::CursorShape::PointingHandCursor);
    resize(200, 200);
}

void PreciseLandingAssistCard::init_signal_slots()
{

}

void PreciseLandingAssistCard::init_timers()
{
    connect(&_tm_update, &QTimer::timeout, this, &PreciseLandingAssistCard::tm_update_slot);

    _tm_update.setInterval(1000 / 10);
    _tm_update.start();
}

void PreciseLandingAssistCard::resizeEvent(QResizeEvent *e)
{
    cs::CSWidget::resizeEvent(e);

    _ctrl->setGeometry(rect());
}

void PreciseLandingAssistCard::mousePressEvent(QMouseEvent *e)
{
    _pt_offset = e->pos();
}

void PreciseLandingAssistCard::mouseMoveEvent(QMouseEvent *e)
{
    auto pt = e->pos() - _pt_offset;
    pt = mapToParent(pt);

    this->move(pt);
}

GLPoint2f PreciseLandingAssistCard::calc_uav_pos() const
{
    GLPoint2f pt;

//    aos::AlgorithmsKdtreeUtilities::lonlat_distance();
//    lonlat_direction();

    return pt;
}

/**
 * @brief PreciseLandingAssistCard::parse_data_by_uri
 * @param jo
 * @param uri
 * @return
 */
QJsonValue PreciseLandingAssistCard::parse_data_by_uri(const QJsonObject &jo, QStringList &uri)
{
    // try to match pack alias
    auto packAlias = jo.value(str_pack_alias).toString();
    if (uri.isEmpty() || uri.first() != packAlias)
    {
        return QJsonValue::Null;
    }

    // operate temperary var
    uri.pop_front();

    // traverse uri
    auto data = jo.value(str_states);
    for (auto subUri : uri)
    {
        data = data.toObject().value(subUri);
    }

    return data;
}

void PreciseLandingAssistCard::tm_update_slot()
{
    calc_uav_pos();

    // to do
}



}   // solo
