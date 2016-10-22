/* ---------------------------------------------------------------- *
   Author: Antti Jumpponen <kuumies@gmail.com>
   Desc:   Definition of kuu::opengl::Quad class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>

namespace kuu
{
namespace opengl
{

/* ---------------------------------------------------------------- *
   A quad mesh.
 * ---------------------------------------------------------------- */
class Quad
{
public:
    // Defines a shared pointer of quad.
    using Ptr = std::shared_ptr<Quad>;

    // Constructs the quad.
    Quad(float width = 1.0f, float height = 1.0f);

    // Updates the quad
    void update(float elapsed);

    // Renders the quad
    void render();

private:
    struct Data;
    std::shared_ptr<Data> d;
};

} // namespace opengl
} // namespace kuu
