#pragma once
#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

#include "Projectile.h"
#include "EventSystem.h"
#include "ProjectileData.h"

class ProjectileManager : public Listener
{
public:
	ProjectileManager();
	~ProjectileManager();
	
	void Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat);
	void InitialiseFromFile(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& filename);
	void Update(const float dt);
	void Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix );

	void LoadProjectileData(const std::string& sFilename);
	std::vector<std::shared_ptr<Projectile>> CreateProjectilePool(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons);
	
	void SetProjectilePool(std::vector<std::shared_ptr<Projectile>> vecProjectilePool) { m_vecProjectilePool = vecProjectilePool; }
	inline void SetDelay(const float fDelay) noexcept { m_fDelay = fDelay; }
	inline void SetProjectileImagePath(const std::string& sPath) noexcept { m_sProjectileImagePath = sPath; }

	void SpawnProjectile(Vector2f vSpawnPosition, float fLifeTime);
	void SpawnProjectiles(Vector2f vSpawnPosition);
	void SpawnProjectiles(Vector2f vSpawnPosition, Vector2f vTargetPosition);
	
	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;
	
private:
	void SpawnProjectile();
	std::shared_ptr<Projectile> GetFreeProjectile();

	std::string m_sProjectileImagePath;
	float m_fLifeTime;
	float m_fDelay;
	float m_fCounter;
	Vector2f m_vSpawnPosition;
	Vector2f m_vTargetPosition;
	
	std::vector<std::shared_ptr<Projectile>> m_vecProjectilePool;
};

#endif // !PROJECTILEMANAGER_H