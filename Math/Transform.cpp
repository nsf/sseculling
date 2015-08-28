#include "Math/Transform.h"

Mat4 to_mat4(const Transform &tf)
{
	return to_mat4(tf.orientation) * Mat4_Translate(tf.translation);
}

Transform inverse(const Transform &tf)
{
	return {inverse(tf.orientation), -tf.translation};
}

Vec3f transform(const Vec3f &in, const Transform &tr)
{
	return tr.orientation.rotate(in) + tr.translation;
}

Transform transform(const Transform &in, const Transform &tr)
{
	Transform out;
	out.translation = tr.translation + tr.orientation.rotate(in.translation);
	out.orientation = in.orientation * tr.orientation;
	return out;
}
