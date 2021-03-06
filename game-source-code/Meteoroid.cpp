/////////////////////////////////////////////////////////////////////
/// \brief   Meteoroid Entity (cannot be destroyed by bullets)
/////////////////////////////////////////////////////////////////////

#include "Meteoroid.hpp"

Meteoroid::Meteoroid(const Vector2i resolution,
               float distanceFromCentre,
               float angle,
               float scale,
               const entity::ID type,
               const TextureHolder &textureHolder,
               const textures::ID id) : Entity{resolution,
                                               distanceFromCentre,
                                               angle,
                                               scale,
                                               type,
                                               textureHolder}
{
    _id = id;
    _lives = 1;
    _sprite.setTexture(textureHolder.get(_id));
    _sprite.setOrigin(_sprite.getGlobalBounds().width / 2, _sprite.getGlobalBounds().height / 2);
    _sprite.setScale(_scale, _scale);
    _sprite.setRotation(-_angle);
    _isMoving = true;
    setMove(0); // Spawn at centre of screen
    update();
}

void Meteoroid::setMove(float distance)
{
    _isMoving = true;
    _futureMoveValue = distance;
    _rotationOffset++;
}

void Meteoroid::reset()
{
    _isMoving = false;
    _sprite.setPosition(_resolution.x*2,_resolution.y*2); // Move offscreen?
    _sprite.setScale(0,0);
}

void Meteoroid::update()
{
    if (_isMoving)
    {
        _sprite.setRotation(_rotationOffset);
        move();
    }
}

const float Meteoroid::getRadius() const
{
    auto mid = Vector2f{_resolution.x/2.f, _resolution.y/2.f};
    auto pos = Vector2f{_sprite.getPosition().x, _sprite.getPosition().y};
    Vector2f length = mid-pos;
    return sqrt((length.x * length.x) + (length.y * length.y));
}

const float Meteoroid::getAngle() const
{
    return _angle;
}

const float Meteoroid::getDistanceFromCentre() const
{
    return _distanceFromCentre - _sprite.getGlobalBounds().height/2;
}

const Vector2f Meteoroid::getPosition() const
{
    return _sprite.getPosition();
}

const Sprite& Meteoroid::getSprite() const
{
    return _sprite;
}

const Vector2f Meteoroid::getScale() const
{
    return _sprite.getScale();
}

const int Meteoroid::getLives() const
{
    return _lives;
}

const entity::ID Meteoroid::getType() const
{
    return _type;
}

const bool Meteoroid::isAlive() const
{
    return _isAlive;
}

void Meteoroid::die()
{
    _lives--;
    if (_lives==0)
    {
        reset();
    }
}

void Meteoroid::move()
{

    auto offset = 0.f;
    if(_distanceFromCentre==0)
        offset = float(_resolution.x*0.2);

    auto depthScale = ((_distanceFromCentre + offset)/(_resolution.y/2));
    _distanceFromCentre += _futureMoveValue * depthScale;
    auto x_pos = _distanceFromCentre * sin(common::degreeToRad(_angle));
    auto y_pos = _distanceFromCentre * cos(common::degreeToRad(_angle));
    auto scale = 1 + ((getRadius() - (_resolution.y / 2)) / (_resolution.y / 2));
    _sprite.setPosition(x_pos+(_resolution.x / 2),y_pos+(_resolution.y / 2));
    _sprite.setScale(scale * _scale,scale * _scale);

    // Dimming
    auto dimColor = (scale*200) + 55;
    if (getRadius() >= (_resolution.y/2))

        _sprite.setColor(Color(dimColor,dimColor,dimColor));
}