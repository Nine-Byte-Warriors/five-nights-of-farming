#pragma once
#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include "Projectile.h"
#include "EventSystem.h"

class ProjectileManager : public Listener
{
public:
	ProjectileManager();
	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void Update(const float dt);
	void Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix );

	inline std::shared_ptr<Projectile> GetProjectile() const noexcept { return m_projectile; } // single projectile
	//inline std::shared_ptr<Projectile> GetProjectiles() const noexcept { return m_projectiles; } // multiple projectiles
	
	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;
	
private:
	void SpawnProjectile();

	float m_fLifeTime;
	Vector2f m_vDirection;
	bool m_bRenderProjectiles;
	Vector2f m_vSpawnPosition;
	Vector2f m_vTargetPosition;
	std::shared_ptr<Projectile> m_projectile; //DEBUG: should be a collection from a bullet pool
	//std::vector<std::shared_ptr<Projectile>> m_projectiles;
};

#endif // !PROJECTILEMANAGER_H