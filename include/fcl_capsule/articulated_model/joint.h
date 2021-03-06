/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/** \author Dalibor Matura, Jia Pan */

#ifndef FCL_ARTICULATED_MODEL_JOINT_H
#define FCL_ARTICULATED_MODEL_JOINT_H

#include "fcl_capsule/math/transform.h"
#include "fcl_capsule/data_types.h"

#include <string>
#include <vector>
#include <map>
#include <limits>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace fcl_capsule
{

class JointConfig;
class Link;

enum JointType {JT_UNKNOWN, JT_PRISMATIC, JT_REVOLUTE, JT_BALLEULER};

/// @brief Base Joint
class Joint
{
public:

  Joint(const boost::shared_ptr<Link>& link_parent, const boost::shared_ptr<Link>& link_child,
        const Transform3f& transform_to_parent,
        const std::string& name);

  Joint(const std::string& name);

  virtual ~Joint() {}

  const std::string& getName() const;
  void setName(const std::string& name);

  virtual Transform3f getLocalTransform() const = 0;

  virtual std::size_t getNumDofs() const = 0;

  boost::shared_ptr<JointConfig> getJointConfig() const;
  void setJointConfig(const boost::shared_ptr<JointConfig>& joint_cfg);

  boost::shared_ptr<Link> getParentLink() const;
  boost::shared_ptr<Link> getChildLink() const;

  void setParentLink(const boost::shared_ptr<Link>& link);
  void setChildLink(const boost::shared_ptr<Link>& link);

  JointType getJointType() const;

  const Transform3f& getTransformToParent() const;
  void setTransformToParent(const Transform3f& t);
  
protected:

  /// links to parent and child are only for connection, so weak_ptr to avoid cyclic dependency
  boost::weak_ptr<Link> link_parent_, link_child_;

  JointType type_;

  std::string name_;
  
  boost::shared_ptr<JointConfig> joint_cfg_;

  Transform3f transform_to_parent_;
};


class PrismaticJoint : public Joint
{
public:
  PrismaticJoint(const boost::shared_ptr<Link>& link_parent, const boost::shared_ptr<Link>& link_child,
                 const Transform3f& transform_to_parent,
                 const std::string& name,
                 const Vec3f& axis);

  virtual ~PrismaticJoint() {}

  Transform3f getLocalTransform() const;

  std::size_t getNumDofs() const;

  const Vec3f& getAxis() const;

protected:
  Vec3f axis_;
};

class RevoluteJoint : public Joint
{
public:
  RevoluteJoint(const boost::shared_ptr<Link>& link_parent, const boost::shared_ptr<Link>& link_child,
                const Transform3f& transform_to_parent,
                const std::string& name,
                const Vec3f& axis);

  virtual ~RevoluteJoint() {}

  Transform3f getLocalTransform() const;

  std::size_t getNumDofs() const;

  const Vec3f& getAxis() const;

protected:
  Vec3f axis_;
};



class BallEulerJoint : public Joint
{
public:
  BallEulerJoint(const boost::shared_ptr<Link>& link_parent, const boost::shared_ptr<Link>& link_child,
                 const Transform3f& transform_to_parent,
                 const std::string& name);

  virtual ~BallEulerJoint() {}

  std::size_t getNumDofs() const;

  Transform3f getLocalTransform() const; 
};


}

#endif
